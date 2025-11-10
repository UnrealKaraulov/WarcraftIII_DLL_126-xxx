// ModelPatcher.h
#pragma once
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cstdint>

#pragma pack(push,1)



struct Mdx_FLOAT3 { float x, y, z; };

struct Mdx_Sequence {
    char Name[80];
    int IntervalStart;
    int IntervalEnd;
    float MoveSpeed;
    unsigned int Flags;
    //0 - Looping
    //1 - NonLooping
    float Rarity;
    unsigned int SyncPoint;
    float BoundsRadius;
    Mdx_FLOAT3 MinimumExtent;
    Mdx_FLOAT3 MaximumExtent;
};

struct Mdx_Track {
    int NrOfTracks;
    int InterpolationType;
    //0 - None
    //1 - Linear
    //2 - Hermite
    //3 - Bezier
    unsigned int GlobalSequenceId;
};

struct Mdx_Tracks {
    int NrOfTracks;
    unsigned int GlobalSequenceId;
};

struct Mdx_Node {
    unsigned int InclusiveSize;
    char Name[80];
    unsigned int ObjectId;
    unsigned int ParentId;
    unsigned int Flags;

    //0        - Helper
    //#1       - DontInheritTranslation
    //#2       - DontInheritRotation
    //#4       - DontInheritScaling
    //#8       - Billboarded
    //#16      - BillboardedLockX
    //#32      - BillboardedLockY
    //#64      - BillboardedLockZ
    //#128     - CameraAnchored
    //#256     - Bone
    //#512     - Light
    //#1024    - EventObject
    //#2048    - Attachment
    //#4096    - ParticleEmitter
    //#8192    - CollisionShape
    //#16384   - RibbonEmitter
    //#32768   - Unshaded / EmitterUsesMdl
    //#65536   - SortPrimitivesFarZ / EmitterUsesTga
    //#131072  - LineEmitter
    //#262144  - Unfogged
    //#524288  - ModelSpace
    //#1048576 - XYQuad
};

struct Mdx_GeosetAnimation {
    unsigned int InclusiveSize;
    float Alpha;
    unsigned int Flags;
    //#1 - DropShadow
    //#2 - Color
    Mdx_FLOAT3 Color;
    unsigned int GeosetId;
};

struct Mdx_Texture {
    int ReplaceableId;
    char FileName[260];
    unsigned int Flags;
    //#1 - WrapWidth
    //#2 - WrapHeight
};

struct Mdx_SequenceTime {
    int* IntervalStart;
    int* IntervalEnd;
};
#pragma pack(pop)

class ModelPatcher {
public:
    // queues (public API mirrors your helper functions)
    struct CollisionFix { std::string filePath; float X, Y, Z, Radius; };
    struct TextureFix  { std::string filePath; int TextureID; std::string NewTexturePath; };
    struct Patch       { std::string filePath; std::string patchPath; std::vector<unsigned char> patchData; };
    struct TagRemove   { std::string filePath; std::string TagName; };
    struct ReSpeed     { std::string filePath; std::string AnimationName; float SpeedUp; };
    struct SeqValue    { std::string filePath; std::string AnimationName; int Indx; float Value; };
    struct ScaleFix    { std::string filePath; float Scale, ScaleX, ScaleY, ScaleZ; bool uniform; };

    // push tasks
    int setCollisionSphere(const char* mdlpath, float x, float y, float z, float r) {
        collisions_.push_back({mdlpath, x, y, z, r}); return 0;
    }
    int fixTexture(const char* mdlpath, int textureId, const char* newPath) {
        textures_.push_back({mdlpath, textureId, newPath}); return 0;
    }
    int addPatch(const char* mdlpath, const char* patchPath, const std::vector<unsigned char>& data) {
        patches_.push_back({mdlpath, patchPath, data}); return 0;
    }
    int removeTag(const char* mdlpath, const char* tag) {
        removes_.push_back({mdlpath, tag}); return 0;
    }
    int changeAnimationSpeed(const char* mdlpath, const char* seq, float speedUp) {
        respeeds_.push_back({mdlpath, seq, speedUp}); return 0;
    }
    int setSequenceValue(const char* mdlpath, const char* seq, int idx, float value) {
        if (idx < 0 || idx > 6) return -1;
        seqvals_.push_back({mdlpath, seq, idx, value}); return 0;
    }
    int setModelScale(const char* mdlpath, float scale) {
        scalers_.push_back({mdlpath, scale, 0.f, 0.f, 0.f, true}); return 0;
    }
    int setModelScaleEx(const char* mdlpath, float sx, float sy, float sz) {
        scalers_.push_back({mdlpath, 0.f, sx, sy, sz, false}); return 0;
    }

    // core processing: in-place patch into buffer, resize as needed
    // returns true if modified
    bool processModel(const std::string& filename, std::vector<unsigned char>& data) {
        if (!isMDLX(data)) return false;
        bool changed = false;

        changed |= applySequenceValues(filename, data);
        changed |= applySequenceRespeed(filename, data);
        changed |= applyRemoveTags(filename, data);
        changed |= applyScale(filename, data);    
        changed |= applyCollisionFixes(filename, data);
        changed |= applyTextureFixes(filename, data);
        changed |= applyAppendPatches(filename, data);

        return changed;
    }

    void removeProcessedTasks(const std::string& filename) {
        auto removeMatching = [&](auto& container) {
            container.erase(
                std::remove_if(container.begin(), container.end(),
                    [&](const auto& item) { return item.filePath == filename; }),
                container.end()
            );
            };

        removeMatching(seqvals_);
        removeMatching(respeeds_);
        removeMatching(removes_);
        removeMatching(scalers_);
        removeMatching(textures_);
        removeMatching(collisions_);
        removeMatching(patches_);
    }

    void clearAll() {
        collisions_.clear();
        textures_.clear();
        patches_.clear();
        removes_.clear();
        respeeds_.clear();
        seqvals_.clear();
        scalers_.clear();
    }

private:
    // queues
    std::vector<CollisionFix> collisions_;
    std::vector<TextureFix>   textures_;
    std::vector<Patch>        patches_;
    std::vector<TagRemove>    removes_;
    std::vector<ReSpeed>      respeeds_;
    std::vector<SeqValue>     seqvals_;
    std::vector<ScaleFix>     scalers_;

    static bool isMDLX(const std::vector<unsigned char>& d) {
        return d.size() >= 4 && std::memcmp(d.data(), "MDLX", 4) == 0;
    }

    template<typename F>
    static void iterateChunks(std::vector<unsigned char>& d, F&& f) {
        size_t off = 4;
        while (off + 8 <= d.size()) {
            const char* tag = reinterpret_cast<const char*>(d.data() + off);
            off += 4;

            if (off + 4 > d.size()) break; 

            int len = 0;
            std::memcpy(&len, d.data() + off, 4);
            off += 4;

            if (len < 0 || off + (size_t)len > d.size()) break;

            size_t payload = off;
            if (!f(tag, len, payload)) break;
            off += (size_t)len;
        }
    }

    static void collectNodeTrackTimes(std::vector<unsigned char>& d, size_t nodeOff, std::vector<int*>& timesOut) {
        if (nodeOff + sizeof(Mdx_Node) > d.size()) return;
        size_t offset = nodeOff + sizeof(Mdx_Node);

        auto tryTrack = [&](const char* ttag, int nonInterpSize, int interpSize) {
            if (offset + 8 > d.size()) return;
            if (std::memcmp(d.data() + offset, ttag, 4) != 0) return;
            offset += 4;
            if (offset + sizeof(Mdx_Track) > d.size()) return;
            Mdx_Track tr{}; std::memcpy(&tr, d.data() + offset, sizeof(Mdx_Track));
            offset += sizeof(Mdx_Track);
            int stride = (tr.InterpolationType > 1) ? interpSize : nonInterpSize;
            for (int i = 0; i < tr.NrOfTracks; ++i) {
                if (offset + 4 > d.size()) break;
                timesOut.push_back(reinterpret_cast<int*>(d.data() + offset));
                offset += (size_t)stride;
            }
        };

        tryTrack("KGTR", 16, 40);
        tryTrack("KGRT", 20, 52);
        tryTrack("KGSC", 16, 40);
        tryTrack("KGAO", 8, 16);
        tryTrack("KGAC", 8, 16);
    }

    // 1) SEQS value patch
    bool applySequenceValues(const std::string& filename, std::vector<unsigned char>& d) {
        bool changed = false;
        for (size_t i = 0; i < seqvals_.size();) {
            const auto sv = seqvals_[i];
            if (sv.filePath != filename) { ++i; continue; }
            iterateChunks(d, [&](const char* tag, int len, size_t payload) {
                if (std::memcmp(tag, "SEQS", 4) != 0) return true;
                size_t off = payload;
                size_t end = payload + (size_t)len;
                while (off + sizeof(Mdx_Sequence) <= end) {
                    Mdx_Sequence seq{}; std::memcpy(&seq, d.data() + off, sizeof(Mdx_Sequence));
                    if (sv.AnimationName.empty() || sv.AnimationName == seq.Name) {
                        size_t patchOff = off + 104 + (size_t)sv.Indx * 4;
                        if (patchOff + 4 <= d.size()) {
                            std::memcpy(d.data() + patchOff, &sv.Value, 4);
                            changed = true;
                        }
                    }
                    off += sizeof(Mdx_Sequence);
                }
                return true;
            });
            seqvals_.erase(seqvals_.begin() + (ptrdiff_t)i);
        }
        return changed;
    }

    // 2) respeed + retime all nodes
    bool applySequenceRespeed(const std::string& filename, std::vector<unsigned char>& d) {
        bool changed = false;
        for (size_t i = 0; i < respeeds_.size();) {
            const auto rs = respeeds_[i];
            if (rs.filePath != filename) { ++i; continue; }

            std::vector<Mdx_SequenceTime> seqTimes;
            std::vector<int*> trackTimes;
            int targetIdx = -1;
            int seqIdx = 0;

            iterateChunks(d, [&](const char* tag, int len, size_t payload) {
                size_t end = payload + (size_t)len;

                if (std::memcmp(tag, "SEQS", 4) == 0) {
                    size_t off = payload;
                    while (off + sizeof(Mdx_Sequence) <= end) {
                        Mdx_Sequence seq{}; std::memcpy(&seq, d.data() + off, sizeof(Mdx_Sequence));
                        Mdx_SequenceTime st{
                            reinterpret_cast<int*>(d.data() + off + 80),
                            reinterpret_cast<int*>(d.data() + off + 84)
                        };
                        if (rs.AnimationName == seq.Name) targetIdx = seqIdx;
                        seqTimes.push_back(st);
                        ++seqIdx;
                        off += sizeof(Mdx_Sequence);
                    }
                }

                auto scanNodeList = [&](const char* nodeTag, bool hasSizePrefix) {
                    if (std::memcmp(tag, nodeTag, 4) != 0) return;
                    size_t off = payload;
                    while (off < end) {
                        size_t nodeOff = off + (hasSizePrefix ? 4 : 0);
                        if (nodeOff + sizeof(Mdx_Node) > d.size()) break;
                        collectNodeTrackTimes(d, nodeOff, trackTimes);
                        unsigned int inc = 0; std::memcpy(&inc, d.data() + nodeOff, sizeof(unsigned int));
                        off += (hasSizePrefix ? 4 : 0) + inc;
                    }
                };

                scanNodeList("BONE", true);
                scanNodeList("HELP", false);
                scanNodeList("LITE", true);
                scanNodeList("ATCH", true);
                scanNodeList("PREM", true);
                scanNodeList("PRE2", true);
                scanNodeList("RIBB", true);
                scanNodeList("EVTS", false);
                scanNodeList("CLID", false);

                if (std::memcmp(tag, "GEOA", 4) == 0) {
                    size_t off = payload;
                    while (off + sizeof(Mdx_GeosetAnimation) <= end) {
                        Mdx_GeosetAnimation g{}; std::memcpy(&g, d.data() + off, sizeof(Mdx_GeosetAnimation));
                        collectNodeTrackTimes(d, off + sizeof(Mdx_GeosetAnimation), trackTimes);
                        off += g.InclusiveSize;
                    }
                }

                return true;
            });

            if (targetIdx != -1 && targetIdx < (int)seqTimes.size()) {
                int s = *seqTimes[(size_t)targetIdx].IntervalStart;
                int e = *seqTimes[(size_t)targetIdx].IntervalEnd;
                int newEnd = s + (int)((e - s) / rs.SpeedUp);
                int add = newEnd - e;

                for (auto& st : seqTimes) {
                    if (*st.IntervalStart >= e) {
                        *st.IntervalStart += add;
                        *st.IntervalEnd += add;
                    }
                }
                *seqTimes[(size_t)targetIdx].IntervalEnd = newEnd;

                for (int* t : trackTimes) {
                    if (!t) continue;
                    int v = *t;
                    if (v >= e) *t = v + add;
                    else if (v >= s) *t = s + (int)((float)(v - s) / rs.SpeedUp);
                }
                changed = true;
            }

            respeeds_.erase(respeeds_.begin() + (ptrdiff_t)i);
        }
        return changed;
    }

    // 3) remove tag
    bool applyRemoveTags(const std::string& filename, std::vector<unsigned char>& d) {
        bool changed = false;
        for (size_t i = 0; i < removes_.size();) {
            const auto rm = removes_[i];
            if (rm.filePath != filename) { ++i; continue; }

            size_t tagStart = 0, tagSize = 0;

            // ТОЧНО как в старом коде - просто находим тег
            iterateChunks(d, [&](const char* tag, int len, size_t payload) {
                if (std::memcmp(tag, rm.TagName.c_str(), 4) == 0) {
                    tagStart = payload - 8;    // начало тега (4 байта тега + 4 байта размера)
                    tagSize = (size_t)len + 8; // размер тега с заголовком
                    return false;
                }
                return true;
                });

            if (tagSize > 0 && tagStart + tagSize <= d.size()) {
                // ТОЧНО как в старом коде - грубый memmove
                size_t tailStart = tagStart + tagSize;
                size_t tailSize = d.size() - tailStart;

                if (tailSize > 0) {
                    std::memmove(d.data() + tagStart, d.data() + tailStart, tailSize);
                }

                d.resize(d.size() - tagSize);
                changed = true;

            }
            removes_.erase(removes_.begin() + (ptrdiff_t)i);
        }
        return changed;
    }

    // 4) TEXS texture fix
    bool applyTextureFixes(const std::string& filename, std::vector<unsigned char>& d) {
        bool changed = false;
        for (size_t i = 0; i < textures_.size();) {
            const auto tf = textures_[i];
            if (tf.filePath != filename) { ++i; continue; }
            iterateChunks(d, [&](const char* tag, int len, size_t payload) {
                if (std::memcmp(tag, "TEXS", 4) != 0) return true;
                size_t off = payload;
                size_t end = payload + (size_t)len;
                int idx = 1;
                while (off + sizeof(Mdx_Texture) <= end) {
                    Mdx_Texture t{}; std::memcpy(&t, d.data() + off, sizeof(Mdx_Texture));
                    if (tf.TextureID == idx) {
                        if (tf.NewTexturePath.length() > 3) {
                            t.ReplaceableId = 0;
                            std::memset(t.FileName, 0, sizeof(t.FileName));
                            std::strncpy(t.FileName, tf.NewTexturePath.c_str(), sizeof(t.FileName) - 1);
                            t.FileName[sizeof(t.FileName) - 1] = '\0';
                        } else {
                            t.ReplaceableId = std::atoi(tf.NewTexturePath.c_str());
                            std::memset(t.FileName, 0, sizeof(t.FileName));
                        }
                        std::memcpy(d.data() + off, &t, sizeof(Mdx_Texture));
                        changed = true;
                        break;
                    }
                    ++idx;
                    off += sizeof(Mdx_Texture);
                }
                return true;
            });
            textures_.erase(textures_.begin() + (ptrdiff_t)i);
        }
        return changed;
    }

    // 5) CLID sphere
    bool applyCollisionFixes(const std::string& filename, std::vector<unsigned char>& d) {
        bool changed = false;
        for (size_t i = 0; i < collisions_.size();) {
            const auto cf = collisions_[i];
            if (cf.filePath != filename) { ++i; continue; }
            iterateChunks(d, [&](const char* tag, int len, size_t payload) {
                if (std::memcmp(tag, "CLID", 4) != 0) return true;
                size_t off = payload;
                size_t end = payload + (size_t)len;
                if (off + 12 > end) return true;
                int size1=0,size2=0,shape=0;
                std::memcpy(&size1, d.data() + off, 4); off += 4;
                std::memcpy(&size2, d.data() + off, 4); off += 4;
                std::memcpy(&shape, d.data() + off, 4); off += 4;
                if (shape == 2 && off + 16 <= end) {
                    std::memcpy(d.data() + off + 0,  &cf.X, 4);
                    std::memcpy(d.data() + off + 4,  &cf.Y, 4);
                    std::memcpy(d.data() + off + 8,  &cf.Z, 4);
                    std::memcpy(d.data() + off + 12, &cf.Radius, 4);
                    changed = true;
                }
                return true;
            });
            collisions_.erase(collisions_.begin() + (ptrdiff_t)i);
        }
        return changed;
    }

    // helper blocks for scale
    static const unsigned char* Helper1() {
        static const unsigned char H1[] = {
            0x42,0x4F,0x4E,0x45,0x88,0x00,0x00,0x00,0x80,0x00,
            0x00,0x00,0x42,0x6F,0x6E,0x65,0x5F,0x52,0x6F,0x6F,
            0x74,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00
        };
        return H1;
    }
    static const unsigned char* Helper2() {
        static const unsigned char H2[] = {
            0xFF,0xFF,0xFF,0xFF,0x00,0x01,0x00,0x00,0x4B,
            0x47,0x53,0x43,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
        };
        return H2;
    }
    static const unsigned char* Helper3() {
        static const unsigned char H3[] = { 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF };
        return H3;
    }

    // 6) scale: reparent -1 -> newId, optional pivot insert, append Bone_Root block + scales, GLBS if absent
    bool applyScale(const std::string& filename, std::vector<unsigned char>& d) {
        bool changed = false;
        for (size_t i = 0; i < scalers_.size();) {
            const auto sc = scalers_[i];
            if (sc.filePath != filename) { ++i; continue; }

            unsigned long maxId = 0;
            std::vector<unsigned long*> parentPtrs;
            bool hasGLBS = false;
            size_t pivotInsertPos = 0;

            iterateChunks(d, [&](const char* tag, int len, size_t payload) {
                size_t end = payload + (size_t)len;
                if (std::memcmp(tag, "GLBS", 4) == 0) hasGLBS = true;
                if (std::memcmp(tag, "PIVT", 4) == 0) {
                    size_t pivotSizePos = payload - 4;
                    unsigned int currentSize = 0;
                    std::memcpy(&currentSize, d.data() + pivotSizePos, 4);
                    currentSize += 12;
                    std::memcpy(d.data() + pivotSizePos, &currentSize, 4);
                    pivotInsertPos = payload + currentSize - 12; // вставка в конец payload
                }

                auto scanNodes = [&](const char* nodeTag, bool hasSizePrefix) {
                    if (std::memcmp(tag, nodeTag, 4) != 0) return;
                    size_t off = payload;
                    while (off < end) {
                        size_t nodeOff = off + (hasSizePrefix ? 4 : 0);
                        if (nodeOff + sizeof(Mdx_Node) > d.size()) break;
                        Mdx_Node n{}; std::memcpy(&n, d.data() + nodeOff, sizeof(Mdx_Node));
                        if (n.ObjectId != 0xFFFFFFFF && n.ObjectId > maxId) maxId = n.ObjectId;
                        parentPtrs.push_back(reinterpret_cast<unsigned long*>(d.data() + nodeOff + 88));
                        unsigned int inc = 0; std::memcpy(&inc, d.data() + nodeOff, sizeof(unsigned int));
                        off += (hasSizePrefix ? 4 : 0) + inc;
                    }
                };

                scanNodes("BONE", true);
                scanNodes("HELP", false);
                scanNodes("LITE", true);
                scanNodes("ATCH", true);
                scanNodes("PREM", true);
                scanNodes("PRE2", true);
                scanNodes("RIBB", true);
                scanNodes("EVTS", false);
                scanNodes("CLID", false);
                return true;
            });

            maxId++;
            for (auto* p : parentPtrs) if (*p == 0xFFFFFFFF) *p = maxId;

            std::vector<unsigned char> build;
            build.reserve(d.size() + 256);
            build.insert(build.end(), d.begin(), d.end());

            if (pivotInsertPos != 0 && pivotInsertPos <= build.size()) {
                unsigned char zeros[12] = {0};
                build.insert(build.begin() + (ptrdiff_t)pivotInsertPos, zeros, zeros + 12);
            }

            // Append helper Bone_Root + id + scales
            const unsigned char* H1 = Helper1();
            const unsigned char* H2 = Helper2();
            const unsigned char* H3 = Helper3();
            build.insert(build.end(), H1, H1 + 0x58); // entire H1 length: 88 bytes (0x58)
            unsigned char* idBytes = reinterpret_cast<unsigned char*>(&maxId);
            build.insert(build.end(), idBytes, idBytes + 4);
            build.insert(build.end(), H2, H2 + 0x19); // 25 bytes

            auto pushFloat = [&](float f) {
                unsigned char* pf = reinterpret_cast<unsigned char*>(&f);
                build.insert(build.end(), pf, pf + 4);
            };
            if (sc.uniform) { pushFloat(sc.Scale); pushFloat(sc.Scale); pushFloat(sc.Scale); }
            else { pushFloat(sc.ScaleX); pushFloat(sc.ScaleY); pushFloat(sc.ScaleZ); }

            build.insert(build.end(), H3, H3 + 8);

            if (!hasGLBS) {
                const char glbs[4] = { 'G','L','B','S' };
                build.insert(build.end(), glbs, glbs + 4);
                unsigned long szGLBS = 4;
                unsigned char* p = reinterpret_cast<unsigned char*>(&szGLBS);
                build.insert(build.end(), p, p + 4);
                szGLBS = 0;
                p = reinterpret_cast<unsigned char*>(&szGLBS);
                build.insert(build.end(), p, p + 4);
            }

            d.swap(build);
            changed = true;
            scalers_.erase(scalers_.begin() + (ptrdiff_t)i);
        }
        return changed;
    }

    // 7) append external patches (verbatim bytes)
    bool applyAppendPatches(const std::string& filename, std::vector<unsigned char>& d) {
        bool changed = false;
        for (size_t i = 0; i < patches_.size();) {
            const auto& p = patches_[i];
            if (p.filePath != filename) { ++i; continue; }
            if (!p.patchData.empty()) {
                d.insert(d.end(), p.patchData.begin(), p.patchData.end());
                changed = true;
            }
            patches_.erase(patches_.begin() + (ptrdiff_t)i);
        }
        return changed;
    }
};
