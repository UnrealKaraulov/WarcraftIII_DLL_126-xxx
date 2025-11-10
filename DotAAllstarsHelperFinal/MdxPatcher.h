#pragma once
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cstdint>
#include <stdio.h>
#include <stdarg.h>
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
	struct CollisionFix {
		std::string filePath;
		int Id; // 1-based
		float X, Y, Z, Radius;
		bool IsBox;
		float BoxHalfX, BoxHalfY, BoxHalfZ;
		CollisionFix() : Id(1), X(0), Y(0), Z(0), Radius(0), IsBox(false), BoxHalfX(0), BoxHalfY(0), BoxHalfZ(0) {}
	};
	struct TextureFix { std::string filePath; int TextureID; std::string NewTexturePath; };
	struct Patch { std::string filePath; std::vector<unsigned char> patchData; };
	struct TagRemove { std::string filePath; std::string TagName; };
	struct ReSpeed { std::string filePath; std::string AnimationName; float SpeedUp; };
	struct SeqValue { std::string filePath; std::string AnimationName; int Indx; float Value; };
	struct ScaleFix { std::string filePath; float Scale, ScaleX, ScaleY, ScaleZ; bool uniform; };

	int setCollisionSphere(const char* mdlpath, int id, float x, float y, float z, float r) {
		if (id < 1) id = 1;
		CollisionFix cf; cf.filePath = mdlpath; cf.Id = id; cf.IsBox = false; cf.X = x; cf.Y = y; cf.Z = z; cf.Radius = r;
		collisions_.push_back(cf); return 0;
	}
	int setCollisionBox(const char* mdlpath, int id, float cx, float cy, float cz, float halfX, float halfY, float halfZ) {
		if (id < 1) id = 1;
		CollisionFix cf; cf.filePath = mdlpath; cf.Id = id; cf.IsBox = true; cf.X = cx; cf.Y = cy; cf.Z = cz;
		cf.BoxHalfX = halfX; cf.BoxHalfY = halfY; cf.BoxHalfZ = halfZ;
		collisions_.push_back(cf); return 0;
	}

	int fixTexture(const char* mdlpath, int textureId, const char* newPath) { textures_.push_back({ mdlpath, textureId, newPath }); return 0; }
	int addPatch(const char* mdlpath, const std::vector<unsigned char>& data) { patches_.push_back({ mdlpath, data }); return 0; }
	int removeTag(const char* mdlpath, const char* tag) { removes_.push_back({ mdlpath, tag }); return 0; }
	int changeAnimationSpeed(const char* mdlpath, const char* seq, float speedUp) { respeeds_.push_back({ mdlpath, seq, speedUp }); return 0; }
	int setSequenceValue(const char* mdlpath, const char* seq, int idx, float value) { if (idx < 0 || idx > 15) return -1; seqvals_.push_back({ mdlpath, seq, idx, value }); return 0; }
	int setModelScale(const char* mdlpath, float scale) { scalers_.push_back({ mdlpath, scale, 0.f,0.f,0.f,true }); return 0; }
	int setModelScaleEx(const char* mdlpath, float sx, float sy, float sz) { scalers_.push_back({ mdlpath, 0.f, sx,sy,sz,false }); return 0; }

	// process buffer in-place (returns true if modified)
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
			container.erase(std::remove_if(container.begin(), container.end(),
				[&](const auto& item) { return item.filePath == filename; }), container.end());
			};
		removeMatching(seqvals_); removeMatching(respeeds_); removeMatching(removes_);
		removeMatching(scalers_); removeMatching(textures_); removeMatching(collisions_); removeMatching(patches_);
	}
	void clearAll() { collisions_.clear(); textures_.clear(); patches_.clear(); removes_.clear(); respeeds_.clear(); seqvals_.clear(); scalers_.clear(); }

private:
	std::vector<CollisionFix> collisions_;
	std::vector<TextureFix> textures_;
	std::vector<Patch> patches_;
	std::vector<TagRemove> removes_;
	std::vector<ReSpeed> respeeds_;
	std::vector<SeqValue> seqvals_;
	std::vector<ScaleFix> scalers_;

	static bool isMDLX(const std::vector<unsigned char>& d) { return d.size() >= 4 && std::memcmp(d.data(), "MDLX", 4) == 0; }

	static inline bool ensureAvailable(const std::vector<unsigned char>& d, unsigned int off, unsigned int needed) {
		return off + needed <= d.size();
	}
	static inline bool read_u32_le(const std::vector<unsigned char>& d, unsigned int off, unsigned int& out) {
		if (!ensureAvailable(d, off, 4)) return false;
		out = (unsigned int)d[off] | ((unsigned int)d[off + 1] << 8) | ((unsigned int)d[off + 2] << 16) | ((unsigned int)d[off + 3] << 24);
		return true;
	}
	static inline bool read_i32_le(const std::vector<unsigned char>& d, unsigned int off, int& out) {
		unsigned int v; if (!read_u32_le(d, off, v)) return false; out = static_cast<int>(v); return true;
	}
	static inline bool write_u32_le(std::vector<unsigned char>& d, unsigned int off, unsigned int v) {
		if (!ensureAvailable(d, off, 4)) return false;
		d[off] = (unsigned char)(v & 0xFFu); d[off + 1] = (unsigned char)((v >> 8) & 0xFFu); d[off + 2] = (unsigned char)((v >> 16) & 0xFFu); d[off + 3] = (unsigned char)((v >> 24) & 0xFFu);
		return true;
	}
	static inline bool write_i32_le(std::vector<unsigned char>& d, unsigned int off, int v) { return write_u32_le(d, off, static_cast<unsigned int>(v)); }
	static inline bool write_f32_le(std::vector<unsigned char>& d, unsigned int off, float f) {
		unsigned int u; std::memcpy(&u, &f, 4); return write_u32_le(d, off, u);
	}
	static inline bool read_f32_le(const std::vector<unsigned char>& d, unsigned int off, float& out) {
		unsigned int u; if (!read_u32_le(d, off, u)) return false; std::memcpy(&out, &u, 4); return true;
	}
	static inline bool read_tag(const std::vector<unsigned char>& d, unsigned int off, char out[5]) {
		if (!ensureAvailable(d, off, 4)) return false;
		out[0] = (char)d[off]; out[1] = (char)d[off + 1]; out[2] = (char)d[off + 2]; out[3] = (char)d[off + 3]; out[4] = 0; return true;
	}

	// iterate top-level chunks: tag(4) + size(4) + payload(size)
	template<typename F>
	static void iterateChunks(const std::vector<unsigned char>& d, F&& f) {
		if (d.size() < 8) return;
		unsigned int off = 4; // skip MDLX
		while (off + 8 <= d.size()) {
			char tag[5] = { 0 };
			if (!read_tag(d, off, tag)) break;
			off += 4;
			unsigned int len = 0; if (!read_u32_le(d, off, len)) break;
			off += 4;
			if (len > d.size() - off) break;
			unsigned int payload = off;
			bool cont = f(tag, static_cast<int>(len), payload);
			off += (unsigned int)len;
			if (!cont) break;
		}
	}

	
	static bool findCLIDChunk(const std::vector<unsigned char>& d, unsigned int& payloadPos, unsigned int& sizeFieldPos) {
		payloadPos = SIZE_MAX;
		sizeFieldPos = SIZE_MAX;
		if (d.size() < 8) return false;
		unsigned int off = 4; 
		while (off + 8 <= d.size()) {
			char tag[5] = { 0 };
			if (!read_tag(d, off, tag)) break;
			off += 4;
			unsigned int len = 0;
			if (!read_u32_le(d, off, len)) break;
			unsigned int payload = off + 4;
			if (std::memcmp(tag, "CLID", 4) == 0) {
				payloadPos = payload;
				sizeFieldPos = off;
				return true;
			}
			off = payload + (unsigned int)len;
		}
		return false;
	}

	static bool safeAppendToCLID(std::vector<unsigned char>& d, unsigned int payloadPos, const std::vector<unsigned char>& entry) {
		if (payloadPos == SIZE_MAX) return false;
		if (payloadPos < 4) return false;
		unsigned int sizeFieldPos = payloadPos - 4;
		unsigned int curSize = 0;
		if (!read_u32_le(d, sizeFieldPos, curSize)) return false;
		unsigned int insertPos = payloadPos + curSize;
		if (insertPos > d.size()) return false; 
		d.insert(d.begin() + (ptrdiff_t)insertPos, entry.begin(), entry.end());
		unsigned int newSize = curSize + (unsigned int)entry.size();
		if (!write_u32_le(d, sizeFieldPos, newSize)) return false;
		return true;
	}

	static bool createNewCLIDAtEnd(std::vector<unsigned char>& d, const std::vector<unsigned char>& entry) {
		const char tag[4] = { 'C','L','I','D' };
		unsigned int chunkSize = (unsigned int)entry.size();
		d.insert(d.end(), tag, tag + 4);
		d.push_back((unsigned char)(chunkSize & 0xFF)); d.push_back((unsigned char)((chunkSize >> 8) & 0xFF));
		d.push_back((unsigned char)((chunkSize >> 16) & 0xFF)); d.push_back((unsigned char)((chunkSize >> 24) & 0xFF));
		d.insert(d.end(), entry.begin(), entry.end());
		return true;
	}

	bool applyCollisionFixes(const std::string& filename, std::vector<unsigned char>& d) {
		bool changed = false;
		constexpr unsigned int NODE_NAME_LEN = 80;
		constexpr unsigned int NODE_HEADER_LEN = 4 + NODE_NAME_LEN + 4 + 4 + 4; // InclusiveSize + Name + ObjectId + Parent + Flags

		auto push_u32_le = [&](std::vector<unsigned char>& buf, unsigned int v) {
			buf.push_back((unsigned char)(v & 0xFF)); buf.push_back((unsigned char)((v >> 8) & 0xFF));
			buf.push_back((unsigned char)((v >> 16) & 0xFF)); buf.push_back((unsigned char)((v >> 24) & 0xFF));
			};
		auto push_f32_le = [&](std::vector<unsigned char>& buf, float f) {
			unsigned int u; std::memcpy(&u, &f, 4); push_u32_le(buf, u);
			};

		// Build shape+payload bytes (Box=0, Sphere=2)
		auto buildShapePayload = [&](const CollisionFix& C)->std::vector<unsigned char> {
			std::vector<unsigned char> s;
			push_u32_le(s, C.IsBox ? 0u : 2u); // Box=0, Sphere=2
			if (C.IsBox) {
				// box: 6 floats (center + half extents)
				push_f32_le(s, C.X); push_f32_le(s, C.Y); push_f32_le(s, C.Z);
				push_f32_le(s, C.BoxHalfX); push_f32_le(s, C.BoxHalfY); push_f32_le(s, C.BoxHalfZ);
			}
			else {
				// sphere: 3 floats (center) + radius
				push_f32_le(s, C.X); push_f32_le(s, C.Y); push_f32_le(s, C.Z);
				push_f32_le(s, C.Radius);
			}
			return s;
			};

		// Default header (fallback if CLID пуст)
		auto buildDefaultHeader = [&](const CollisionFix& C)->std::vector<unsigned char> {
			std::vector<unsigned char> hdr;
			hdr.resize(NODE_HEADER_LEN, 0);
			const char* name = C.IsBox ? "Collision Box" : "Collision Sphere";
			unsigned int nl = std::strlen(name); if (nl > NODE_NAME_LEN) nl = NODE_NAME_LEN;
			std::memcpy(hdr.data() + 4, name, nl);
			unsigned int offObj = 4 + NODE_NAME_LEN;
			hdr[offObj + 0] = 0xFF; hdr[offObj + 1] = 0xFF; hdr[offObj + 2] = 0xFF; hdr[offObj + 3] = 0xFF; // ObjectId
			hdr[offObj + 4] = 0xFF; hdr[offObj + 5] = 0xFF; hdr[offObj + 6] = 0xFF; hdr[offObj + 7] = 0xFF; // ParentId
			hdr[offObj + 8] = 0; hdr[offObj + 9] = 0; hdr[offObj + 10] = 0; hdr[offObj + 11] = 0; // Flags
			// InclusiveSize: node header only (0x60)
			unsigned int incl = (unsigned int)NODE_HEADER_LEN;
			hdr[0] = (unsigned char)(incl & 0xFF); hdr[1] = (unsigned char)((incl >> 8) & 0xFF);
			hdr[2] = (unsigned char)((incl >> 16) & 0xFF); hdr[3] = (unsigned char)((incl >> 24) & 0xFF);
			return hdr;
			};

		// Clone header of the first node in CLID (to match file’s style)
		auto cloneHeaderFromCLID = [&](unsigned int payloadPos, unsigned int clidLen)->std::vector<unsigned char> {
			std::vector<unsigned char> hdr;
			if (payloadPos == SIZE_MAX) return hdr;
			unsigned int off = payloadPos;
			unsigned int end = payloadPos + (unsigned int)clidLen;
			if (off + 4 > end) return hdr;
			unsigned int firstIncl = 0;
			if (!read_u32_le(d, off, firstIncl)) return hdr;
			unsigned int hdrLen = (unsigned int)firstIncl;
			if (hdrLen < 4) hdrLen = 4;
			if (hdrLen > 512) hdrLen = 512;
			if (off + hdrLen <= d.size()) {
				hdr.insert(hdr.end(), d.begin() + (ptrdiff_t)off, d.begin() + (ptrdiff_t)(off + hdrLen));
			}
			return hdr;
			};

		for (unsigned int ti = 0; ti < collisions_.size(); ti++) {
			const auto cf = collisions_[ti];
			if (cf.filePath != filename) { ++ti; continue; }

			// Find CLID
			unsigned int payloadPos = SIZE_MAX, sizeFieldPos = SIZE_MAX;
			findCLIDChunk(d, payloadPos, sizeFieldPos);

			bool applied = false;

			if (payloadPos != SIZE_MAX) {
				unsigned int clidLen = 0;
				if (!read_u32_le(d, sizeFieldPos, clidLen)) clidLen = 0;
				unsigned int off = payloadPos;
				unsigned int end = payloadPos + (unsigned int)clidLen;

				int overallIndex = 0;

				while (off + 4 <= end) {
					unsigned int incl = 0;
					if (!read_u32_le(d, off, incl)) break;
					if (incl == 0) break;
					unsigned int nodeStart = off;
					unsigned int nodeEnd = nodeStart + (unsigned int)incl;
					if (nodeEnd > end || nodeEnd > d.size()) break;

					unsigned int shapeOff = nodeEnd;
					if (!ensureAvailable(d, shapeOff, 4)) { off = nodeEnd; continue; }
					int shape = 0;
					if (!read_i32_le(d, shapeOff, shape)) { off = nodeEnd; continue; }

					// Old payload sizes: sphere=16, box=24
					unsigned int oldPayloadSize = (shape == 2) ? 16 : (shape == 0) ? 24 : 0;
					unsigned int oldTotalShapeRange = 4 + oldPayloadSize;

					++overallIndex;

					if (overallIndex == cf.Id) {
						// New shape+payload
						auto newShape = buildShapePayload(cf);
						unsigned int newShapeSize = newShape.size(); // 20 (sphere) или 28 (box)

						if (newShapeSize == oldTotalShapeRange) {
							// Overwrite in-place
							if (shapeOff + newShapeSize <= d.size()) {
								std::memcpy(d.data() + shapeOff, newShape.data(), newShapeSize);
								applied = true; changed = true;
							}
						}
						else {
							// Replace shape+payload range and update only CLID.size 
							if (shapeOff + oldTotalShapeRange <= d.size()) {
								int delta = (int)newShapeSize - (int)oldTotalShapeRange;

								// Replace bytes
								d.erase(d.begin() + (ptrdiff_t)shapeOff, d.begin() + (ptrdiff_t)(shapeOff + oldTotalShapeRange));
								d.insert(d.begin() + (ptrdiff_t)shapeOff, newShape.begin(), newShape.end());

								// Update CLID.size
								unsigned int curClid = 0;
								if (read_u32_le(d, sizeFieldPos, curClid)) {
									unsigned int newClid = (unsigned int)((int)curClid + delta);
									write_u32_le(d, sizeFieldPos, newClid);
									// adjust end
									clidLen = newClid;
									end = payloadPos + (unsigned int)clidLen;
								}

								applied = true; changed = true;
							}
						}
						break;
					}

					// Next entry
					unsigned int next = shapeOff + 4 + oldPayloadSize;
					if (next <= off) next = off + 4;
					if (next > end) break;
					off = next;
				} // while
			}

			if (!applied) {
				// Append one entry
				std::vector<unsigned char> header = buildDefaultHeader(cf);

				if (payloadPos != SIZE_MAX) {
					unsigned int clidLenTmp = 0;
					if (read_u32_le(d, payloadPos - 4, clidLenTmp)) {
						auto cloned = cloneHeaderFromCLID(payloadPos, clidLenTmp);
						if (!cloned.empty()) header = std::move(cloned);
					}
				}

				// Overwrite name (zero-padded)
				if (header.size() >= 4 + NODE_NAME_LEN) {
					std::string nm = cf.IsBox ? "Collision Box0" : "Collision Sphere0";
					nm += std::to_string(cf.Id);
					unsigned int nl = nm.size(); if (nl > NODE_NAME_LEN) nl = NODE_NAME_LEN;
					std::memset(header.data() + 4, 0, NODE_NAME_LEN);
					std::memcpy(header.data() + 4, nm.data(), nl);
				}
				else {
					header.resize(4 + NODE_NAME_LEN, 0);
					std::string nm = cf.IsBox ? "Collision Box0" : "Collision Sphere0";
					nm += std::to_string(cf.Id);

					unsigned int nl = nm.size(); if (nl > NODE_NAME_LEN) nl = NODE_NAME_LEN;
					std::memcpy(header.data() + 4, nm.data(), nl);
				}

				// Full entry = header + shape+payload
				std::vector<unsigned char> entry = header;
				auto shape = buildShapePayload(cf);
				entry.insert(entry.end(), shape.begin(), shape.end());

				if (payloadPos != SIZE_MAX) {
					if (!safeAppendToCLID(d, payloadPos, entry)) {
						createNewCLIDAtEnd(d, entry);
					}
				}
				else {
					createNewCLIDAtEnd(d, entry);
				}
				changed = true;
			}
		}

		return changed;
	}


	bool applyScale(const std::string& filename, std::vector<unsigned char>& d) {
		bool changed = false;
		unsigned char HelperBytesPart1[] = {
							0x42,0x4F,0x4E,0x45,0x88,0x00,0x00,0x00,0x80,0x00,
							0x00,0x00,0x42,0x6F,0x6E,0x65,0x5F,0x52,0x6F,0x6F,
							0x74,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
							0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
							0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
							0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
							0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
							0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
							0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
							0x00,0x00 };

		unsigned char HelperBytesPart2[] = { 0xFF,0xFF,0xFF,0xFF,0x00,0x01,0x00,0x00,0x4B,
									0x47,0x53,0x43,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
									0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };

		unsigned char HelperBytesPart3[] = { 0xFF,0xFF,0xFF,0xFF,
									 0xFF,0xFF,0xFF,0xFF
		};
		static const unsigned int HelperBytesPart1Size = sizeof(HelperBytesPart1);
		static const unsigned int HelperBytesPart2Size = sizeof(HelperBytesPart2);
		static const unsigned int HelperBytesPart3Size = sizeof(HelperBytesPart3);

		for (unsigned int i = 0; i < scalers_.size(); i++) {
			const auto mdlfix = scalers_[i];
			if (mdlfix.filePath != filename) { ++i; continue; }

			char TagName[5];
			memset(TagName, 0, 5);
			unsigned int offset = 0;

			unsigned long MaxObjectId = 0;

			std::vector<unsigned long*> parents;

			unsigned long OffsetToInsertPivotPoint = 0;

			int FoundGLBS = false;
			const char* strGLBS = "GLBS";

			unsigned char* ModelBytes = d.data();

			if (memcmp(&ModelBytes[offset], "MDLX", 4) == 0)
			{
				offset += 4;
				while (offset < d.size())
				{
					std::memcpy(TagName, &ModelBytes[offset], 4);
					if (memcmp(&ModelBytes[offset], strGLBS, 4) == 0)
					{
						offset += 4;
						unsigned int newoffset = offset + *(unsigned int*)&ModelBytes[offset];
						FoundGLBS = true;

						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "PIVT", 4) == 0)
					{
						offset += 4;
						unsigned int newoffset = offset + *(unsigned int*)&ModelBytes[offset];

						*(unsigned int*)&ModelBytes[offset] = 12 + *(unsigned int*)&ModelBytes[offset];

						OffsetToInsertPivotPoint = newoffset + 4;

						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "BONE", 4) == 0)
					{
						offset += 4;
						unsigned int newoffset = offset + *(unsigned int*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;

						while (newoffset > offset)
						{
							std::memcpy(&tmpNode, &ModelBytes[offset], sizeof(Mdx_Node));

							if (tmpNode.ObjectId != 0xFFFFFFFF && tmpNode.ObjectId > MaxObjectId)
							{
								MaxObjectId = tmpNode.ObjectId;
							}
							parents.push_back((unsigned long*)&ModelBytes[offset + 88]);

							offset += tmpNode.InclusiveSize + 8;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "HELP", 4) == 0)
					{
						offset += 4;
						unsigned int newoffset = offset + *(unsigned int*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						while (newoffset > offset)
						{
							std::memcpy(&tmpNode, &ModelBytes[offset], sizeof(Mdx_Node));
							if (tmpNode.ObjectId != 0xFFFFFFFF && tmpNode.ObjectId > MaxObjectId)
							{
								MaxObjectId = tmpNode.ObjectId;
							}
							parents.push_back((unsigned long*)&ModelBytes[offset + 88]);


							offset += tmpNode.InclusiveSize;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "LITE", 4) == 0)
					{

						offset += 4;
						unsigned int newoffset = offset + *(unsigned int*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						while (newoffset > offset)
						{
							unsigned int size_of_this_struct = *(unsigned int*)&ModelBytes[offset];
							offset += 4;
							std::memcpy(&tmpNode, &ModelBytes[offset], sizeof(Mdx_Node));
							if (tmpNode.ObjectId != 0xFFFFFFFF && tmpNode.ObjectId > MaxObjectId)
							{
								MaxObjectId = tmpNode.ObjectId;
							}
							parents.push_back((unsigned long*)&ModelBytes[offset + 88]);

							offset += size_of_this_struct - 4;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "ATCH", 4) == 0)
					{
						offset += 4;
						unsigned int newoffset = offset + *(unsigned int*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						//Mdx_Tracks tmpTracks;
						while (newoffset > offset)
						{
							unsigned int size_of_this_struct = *(unsigned int*)&ModelBytes[offset];
							offset += 4;

							std::memcpy(&tmpNode, &ModelBytes[offset], sizeof(Mdx_Node));

							if (tmpNode.ObjectId != 0xFFFFFFFF && tmpNode.ObjectId > MaxObjectId)
							{
								MaxObjectId = tmpNode.ObjectId;
							}
							parents.push_back((unsigned long*)&ModelBytes[offset + 88]);
							//*( unsigned long* )&ModelBytes[ offset + 88 ] = 0xFFFFFFFF;
							/*offset += tmpNode.InclusiveSize;


							char * attchname = ( char * )&ModelBytes[ offset ];
							offset += 260;


							unsigned long attchid = *( unsigned long * )&ModelBytes[ offset ];

							offset += 4;

							if ( memcmp( &ModelBytes[ offset ], "KATV", 4 ) == 0 )
							{
							offset += 4;
							Mdx_Track tmpTrack;
							std::memcpy( &tmpTrack, &ModelBytes[ offset ], sizeof( Mdx_Track ) );
							offset += sizeof( Mdx_Track );
							for ( unsigned long i = 0; i < tmpTrack.NrOfTracks; i++ )
							{
							offset += ( tmpTrack.InterpolationType > 1 ? 16 : 8 );
							}
							}*/

							offset += size_of_this_struct - 4;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "PREM", 4) == 0)
					{
						offset += 4;
						unsigned int newoffset = offset + *(unsigned int*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						while (newoffset > offset)
						{
							unsigned int size_of_this_struct = *(unsigned int*)&ModelBytes[offset];
							offset += 4;
							std::memcpy(&tmpNode, &ModelBytes[offset], sizeof(Mdx_Node));
							if (tmpNode.ObjectId != 0xFFFFFFFF && tmpNode.ObjectId > MaxObjectId)
							{
								MaxObjectId = tmpNode.ObjectId;
							}
							parents.push_back((unsigned long*)&ModelBytes[offset + 88]);

							offset += size_of_this_struct - 4;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "PRE2", 4) == 0)
					{
						offset += 4;
						unsigned int newoffset = offset + *(unsigned int*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						while (newoffset > offset)
						{
							unsigned int size_of_this_struct = *(unsigned int*)&ModelBytes[offset];
							offset += 4;
							std::memcpy(&tmpNode, &ModelBytes[offset], sizeof(Mdx_Node));

							if (tmpNode.ObjectId != 0xFFFFFFFF && tmpNode.ObjectId > MaxObjectId)
							{
								MaxObjectId = tmpNode.ObjectId;
							}
							parents.push_back((unsigned long*)&ModelBytes[offset + 88]);

							offset += size_of_this_struct - 4;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "RIBB", 4) == 0)
					{
						offset += 4;
						unsigned int newoffset = offset + *(unsigned int*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						while (newoffset > offset)
						{
							unsigned int size_of_this_struct = *(unsigned int*)&ModelBytes[offset];
							offset += 4;
							std::memcpy(&tmpNode, &ModelBytes[offset], sizeof(Mdx_Node));
							if (tmpNode.ObjectId != 0xFFFFFFFF && tmpNode.ObjectId > MaxObjectId)
							{
								MaxObjectId = tmpNode.ObjectId;
							}
							parents.push_back((unsigned long*)&ModelBytes[offset + 88]);

							offset += size_of_this_struct - 4;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "EVTS", 4) == 0)
					{
						offset += 4;
						unsigned int newoffset = offset + *(unsigned int*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						Mdx_Tracks tmpTracks;
						while (newoffset > offset)
						{
							std::memcpy(&tmpNode, &ModelBytes[offset], sizeof(Mdx_Node));
							if (tmpNode.ObjectId != 0xFFFFFFFF && tmpNode.ObjectId > MaxObjectId)
							{
								MaxObjectId = tmpNode.ObjectId;
							}
							parents.push_back((unsigned long*)&ModelBytes[offset + 88]);

							offset += tmpNode.InclusiveSize;
							if (memcmp(&ModelBytes[offset], "KEVT", 4) == 0)
							{
								offset += 4;
								std::memcpy(&tmpTracks, &ModelBytes[offset], sizeof(Mdx_Tracks));
								offset += sizeof(Mdx_Tracks);
								for (int n = 0; n < tmpTracks.NrOfTracks; n++)
								{
									offset += 4;
								}
							}
							else offset += 4;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "CLID", 4) == 0)
					{
						offset += 4;
						unsigned int newoffset = offset + *(unsigned int*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						while (newoffset > offset)
						{
							std::memcpy(&tmpNode, &ModelBytes[offset], sizeof(Mdx_Node));

							if (tmpNode.ObjectId != 0xFFFFFFFF && tmpNode.ObjectId > MaxObjectId)
							{
								MaxObjectId = tmpNode.ObjectId;
							}
							parents.push_back((unsigned long*)&ModelBytes[offset + 88]);

							offset += tmpNode.InclusiveSize;
							unsigned int size_of_this_struct = *(unsigned int*)&ModelBytes[offset];
							offset += 4;
							size_of_this_struct = size_of_this_struct == 0 ? 24 : 16;
							offset += size_of_this_struct;
						}
						offset = newoffset;
					}
					else
					{
						offset += 4;
						offset += *(int*)&ModelBytes[offset];
					}

					offset += 4;
				}
			}


			MaxObjectId++;

			for (unsigned long* parOffsets : parents)
			{
				unsigned long curparent = *parOffsets;
				if (curparent == 0xFFFFFFFF)
				{
					*parOffsets = MaxObjectId;
				}
			}
			std::vector<unsigned char> FullPatchData;

			FullPatchData.insert(FullPatchData.end(), (unsigned char*)(ModelBytes), (unsigned char*)(ModelBytes + d.size()));

			if (OffsetToInsertPivotPoint != 0)
			{
				char ZeroPos[12];
				memset(ZeroPos, 0, sizeof(ZeroPos));
				FullPatchData.insert(FullPatchData.begin() + OffsetToInsertPivotPoint, ZeroPos, ZeroPos + 12);
			}

			FullPatchData.insert(FullPatchData.end(), (unsigned char*)(HelperBytesPart1), (unsigned char*)(HelperBytesPart1 + sizeof(HelperBytesPart1)));
			unsigned char* patchbytes = (unsigned char*)&MaxObjectId;

			FullPatchData.push_back(patchbytes[0]);
			FullPatchData.push_back(patchbytes[1]);
			FullPatchData.push_back(patchbytes[2]);
			FullPatchData.push_back(patchbytes[3]);

			FullPatchData.insert(FullPatchData.end(), (unsigned char*)(HelperBytesPart2), (unsigned char*)(HelperBytesPart2 + sizeof(HelperBytesPart2)));


			float scaleall = mdlfix.Scale;

			if (scaleall != 0.0f)
			{
				patchbytes = (unsigned char*)&mdlfix.Scale;


				FullPatchData.push_back(patchbytes[0]);
				FullPatchData.push_back(patchbytes[1]);
				FullPatchData.push_back(patchbytes[2]);
				FullPatchData.push_back(patchbytes[3]);

				FullPatchData.push_back(patchbytes[0]);
				FullPatchData.push_back(patchbytes[1]);
				FullPatchData.push_back(patchbytes[2]);
				FullPatchData.push_back(patchbytes[3]);

				FullPatchData.push_back(patchbytes[0]);
				FullPatchData.push_back(patchbytes[1]);
				FullPatchData.push_back(patchbytes[2]);
				FullPatchData.push_back(patchbytes[3]);
			}
			else
			{
				patchbytes = (unsigned char*)&mdlfix.ScaleX;


				FullPatchData.push_back(patchbytes[0]);
				FullPatchData.push_back(patchbytes[1]);
				FullPatchData.push_back(patchbytes[2]);
				FullPatchData.push_back(patchbytes[3]);

				patchbytes = (unsigned char*)&mdlfix.ScaleY;

				FullPatchData.push_back(patchbytes[0]);
				FullPatchData.push_back(patchbytes[1]);
				FullPatchData.push_back(patchbytes[2]);
				FullPatchData.push_back(patchbytes[3]);

				patchbytes = (unsigned char*)&mdlfix.ScaleZ;

				FullPatchData.push_back(patchbytes[0]);
				FullPatchData.push_back(patchbytes[1]);
				FullPatchData.push_back(patchbytes[2]);
				FullPatchData.push_back(patchbytes[3]);
			}

			FullPatchData.insert(FullPatchData.end(), (unsigned char*)(HelperBytesPart3), (unsigned char*)(HelperBytesPart3 + sizeof(HelperBytesPart3)));

			if (!FoundGLBS)
			{
				FullPatchData.push_back(strGLBS[0]);
				FullPatchData.push_back(strGLBS[1]);
				FullPatchData.push_back(strGLBS[2]);
				FullPatchData.push_back(strGLBS[3]);
				unsigned long szGLBS = 4;
				patchbytes = (unsigned char*)&szGLBS;
				FullPatchData.push_back(patchbytes[0]);
				FullPatchData.push_back(patchbytes[1]);
				FullPatchData.push_back(patchbytes[2]);
				FullPatchData.push_back(patchbytes[3]);
				szGLBS = 0;
				patchbytes = (unsigned char*)&szGLBS;
				FullPatchData.push_back(patchbytes[0]);
				FullPatchData.push_back(patchbytes[1]);
				FullPatchData.push_back(patchbytes[2]);
				FullPatchData.push_back(patchbytes[3]);
			}

			d = FullPatchData;
			changed = true;
		}

		return changed;
	}


	void ProcessNodeAnims(unsigned char* ModelBytes, unsigned int offset, std::vector<int*>& TimesForReplace) {
		Mdx_Track tmpTrack;

		auto grab = [&](const char* tag, int nonInterpSize, int interpSize) -> bool {
			if (memcmp(&ModelBytes[offset], tag, 4) != 0) return false;
			offset += 4;
			std::memcpy(&tmpTrack, &ModelBytes[offset], sizeof(Mdx_Track));
			offset += sizeof(Mdx_Track);

			int stride = (tmpTrack.InterpolationType > 1) ? interpSize : nonInterpSize;
			for (int i = 0; i < tmpTrack.NrOfTracks; ++i) {
				TimesForReplace.push_back(reinterpret_cast<int*>(&ModelBytes[offset])); // Frame(int32)
				offset += stride;
			}
			return true;
			};

		struct TagInfo { const char* tag; int nonInterp; int interp; };
		static const TagInfo tags[] = {
			{"KGTR", 16, 40}, // Node Translation
			{"KGRT", 20, 52}, // Node Rotation
			{"KGSC", 16, 40}, // Node Scaling
			{"KGAO",  8, 16}, // Alpha
			{"KGAC",  8, 16}, // Color
			{"KCTR", 16, 40}, // Camera Translation
			{"KCRL",  8, 16}, // Camera Rotation
			{"KTTR", 16, 40}, // Camera TargetTranslation
			{"KRVS",  8, 16}, // Ribbon Visibility (float1)
			{"KRHA",  8, 16}, // Ribbon HeightAbove (float1)
			{"KRHB",  8, 16}, // Ribbon HeightBelow (float1)
			{"KRAL",  8, 16}, // Ribbon Alpha (float1)
			{"KRTX",  8, 16}  // Ribbon TextureSlot (int1)
		};

		bool found = true;
		while (found) {
			found = false;
			for (const auto& ti : tags) {
				found |= grab(ti.tag, ti.nonInterp, ti.interp);
			}
		}
	}




	bool applySequenceRespeed(const std::string& filename, std::vector<unsigned char>& d) {
		bool changed = false;
		for (unsigned int i = 0; i < respeeds_.size(); i++) {
			const auto rs = respeeds_[i];
			if (rs.filePath != filename) { ++i; continue; }

			int SequenceID = 0;
			int ReplaceSequenceID = -1;
			std::vector<std::pair<int*, int*>> Sequences;
			std::vector<int*> TimesForReplace;

			unsigned int offset = 0;
			unsigned char* ModelBytes = d.data();
			unsigned int sz = d.size();

			auto isDigitsOnly = [](const std::string& s)->bool {
				if (s.empty()) return false;
				for (auto& c : s)
				{
					if (!std::isdigit(static_cast<unsigned char>(c))) return false;
				}
				return true;
				};

			if (memcmp(&ModelBytes[offset], "MDLX", 4) == 0) {
				offset += 4;
				while (offset < sz) {
					if (memcmp(&ModelBytes[offset], "SEQS", 4) == 0) {
						Mdx_Sequence tmpSequence;
						offset += 4;
						unsigned int currenttagsize = *(unsigned int*)&ModelBytes[offset];
						unsigned int SequencesCount = currenttagsize / sizeof(Mdx_Sequence);
						unsigned int newoffset = offset + currenttagsize;
						offset += 4;

						while (SequencesCount > 0) {
							SequencesCount--;
							std::memcpy(&tmpSequence, &ModelBytes[offset], sizeof(Mdx_Sequence));

							if (!rs.AnimationName.empty()) {
								if (isDigitsOnly(rs.AnimationName)) {
									int targetId = std::stoi(rs.AnimationName);
									if (SequenceID == targetId)
									{
										ReplaceSequenceID = SequenceID;
									}
								}
								else {
									if (rs.AnimationName == tmpSequence.Name) ReplaceSequenceID = SequenceID;
								}
							}

							Sequences.push_back({
								(int*)&ModelBytes[offset + 80],
								(int*)&ModelBytes[offset + 84]
								});

							offset += sizeof(Mdx_Sequence);
							SequenceID++;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "BONE", 4) == 0) {
						offset += 4;
						unsigned int newoffset = offset + *(unsigned int*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						while (newoffset > offset) {
							std::memcpy(&tmpNode, &ModelBytes[offset], sizeof(Mdx_Node));
							ProcessNodeAnims(ModelBytes, offset + sizeof(Mdx_Node), TimesForReplace);
							offset += tmpNode.InclusiveSize + 8;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "HELP", 4) == 0) {
						offset += 4;
						unsigned int newoffset = offset + *(unsigned int*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						while (newoffset > offset) {
							std::memcpy(&tmpNode, &ModelBytes[offset], sizeof(Mdx_Node));
							ProcessNodeAnims(ModelBytes, offset + sizeof(Mdx_Node), TimesForReplace);
							offset += tmpNode.InclusiveSize;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "LITE", 4) == 0 ||
						memcmp(&ModelBytes[offset], "ATCH", 4) == 0 ||
						memcmp(&ModelBytes[offset], "PREM", 4) == 0 ||
						memcmp(&ModelBytes[offset], "PRE2", 4) == 0 ||
						memcmp(&ModelBytes[offset], "RIBB", 4) == 0) {
						offset += 4;
						unsigned int newoffset = offset + *(unsigned int*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						while (newoffset > offset) {
							unsigned int size_of_this_struct = *(unsigned int*)&ModelBytes[offset];
							offset += 4;
							std::memcpy(&tmpNode, &ModelBytes[offset], sizeof(Mdx_Node));
							ProcessNodeAnims(ModelBytes, offset + sizeof(Mdx_Node), TimesForReplace);
							offset += size_of_this_struct - 4;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "GEOA", 4) == 0) {
						Mdx_GeosetAnimation tmpGeo;
						offset += 4;
						unsigned int newoffset = offset + *(unsigned int*)&ModelBytes[offset];
						offset += 4;
						while (newoffset > offset) {
							std::memcpy(&tmpGeo, &ModelBytes[offset], sizeof(Mdx_GeosetAnimation));
							ProcessNodeAnims(ModelBytes, offset + sizeof(Mdx_GeosetAnimation), TimesForReplace);
							offset += tmpGeo.InclusiveSize;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "EVTS", 4) == 0) {
						offset += 4;
						unsigned int newoffset = offset + *(unsigned int*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode; Mdx_Tracks tmpTracks;
						while (newoffset > offset) {
							std::memcpy(&tmpNode, &ModelBytes[offset], sizeof(Mdx_Node));
							ProcessNodeAnims(ModelBytes, offset + sizeof(Mdx_Node), TimesForReplace);
							offset += tmpNode.InclusiveSize;
							if (memcmp(&ModelBytes[offset], "KEVT", 4) == 0) {
								offset += 4;
								std::memcpy(&tmpTracks, &ModelBytes[offset], sizeof(Mdx_Tracks));
								offset += sizeof(Mdx_Tracks);
								for (int n = 0; n < tmpTracks.NrOfTracks; n++) {
									TimesForReplace.push_back((int*)&ModelBytes[offset]);
									offset += 4;
								}
							}
							else offset += 4;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "CLID", 4) == 0) {
						offset += 4;
						unsigned int newoffset = offset + *(unsigned int*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						while (newoffset > offset) {
							std::memcpy(&tmpNode, &ModelBytes[offset], sizeof(Mdx_Node));
							ProcessNodeAnims(ModelBytes, offset + sizeof(Mdx_Node), TimesForReplace);
							offset += tmpNode.InclusiveSize;
							unsigned int size_of_this_struct = *(unsigned int*)&ModelBytes[offset];
							offset += 4;
							size_of_this_struct = size_of_this_struct == 0 ? 24u : 16u;
							offset += size_of_this_struct;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "CAMS", 4) == 0) {
						const int MODEL_CAMERA_NAME_LENGTH = 0x50;
						offset += 4;
						unsigned int newoffset = offset + *(unsigned int*)&ModelBytes[offset];
						offset += 4;
						while (newoffset > offset) {
							unsigned int camSize = *(unsigned int*)&ModelBytes[offset];
							offset += 4;
							unsigned int animStart = offset + MODEL_CAMERA_NAME_LENGTH + (3 * 4) + 4 + 4 + 4 + (3 * 4);
							ProcessNodeAnims(ModelBytes, animStart, TimesForReplace);
							offset += camSize - 4;
						}
						offset = newoffset;
					}
					else {
						offset += 4;
						offset += *(int*)&ModelBytes[offset];
					}
					offset += 4;
				}
			}

			if (ReplaceSequenceID != -1) {
				int SeqEndTime = *Sequences[(unsigned)ReplaceSequenceID].second;
				int SeqStartTime = *Sequences[(unsigned)ReplaceSequenceID].first;
				int NewEndTime = SeqStartTime + (int)((SeqEndTime - SeqStartTime) / rs.SpeedUp);
				int AddTime = NewEndTime - SeqEndTime;

				for (unsigned int n = 0; n < Sequences.size(); n++) {
					if ((int)n == ReplaceSequenceID) continue;
					if (*Sequences[n].first >= SeqEndTime) {
						*Sequences[n].first += AddTime;
						*Sequences[n].second += AddTime;
						changed = true;
					}
				}

				*Sequences[(unsigned int)ReplaceSequenceID].second = NewEndTime;
				changed = true;

				int keysModified = 0;
				for (int* dwTime : TimesForReplace) {
					if (*dwTime >= SeqEndTime) {
						*dwTime += AddTime;
						keysModified++;
						changed = true;
					}
					else if (*dwTime >= SeqStartTime) {
						*dwTime = SeqStartTime + (int)((float)(*dwTime - SeqStartTime) / rs.SpeedUp);
						keysModified++;
						changed = true;
					}
				}
			}

			TimesForReplace.clear();
			Sequences.clear();
		}
		return changed;
	}

	bool applySequenceValues(const std::string& filename, std::vector<unsigned char>& d) {
		bool changed = false;
		for (unsigned int i = 0; i < seqvals_.size(); i++) {
			const auto sv = seqvals_[i];
			if (sv.filePath != filename) { ++i; continue; }
			iterateChunks(d, [&](const char* tag, int len, unsigned int payload) {
				if (std::memcmp(tag, "SEQS", 4) != 0) return true;
				unsigned int off = payload;
				unsigned int end = payload + (unsigned int)len;
				while (off + sizeof(Mdx_Sequence) <= end) {
					Mdx_Sequence seq{}; std::memcpy(&seq, d.data() + off, sizeof(Mdx_Sequence));
					if (sv.AnimationName.empty() || sv.AnimationName == seq.Name) {
						unsigned int patchOff = off + 104 + (unsigned int)sv.Indx * 4;
						if (patchOff + 4 <= d.size()) {
							std::memcpy(d.data() + patchOff, &sv.Value, 4);
							changed = true;
						}
					}
					off += sizeof(Mdx_Sequence);
				}
				return true;
				});
		}
		return changed;
	}

	bool applyRemoveTags(const std::string& filename, std::vector<unsigned char>& d) {
		bool changed = false;
		for (unsigned int i = 0; i < removes_.size(); i++) {
			const auto rm = removes_[i];
			if (rm.filePath != filename) { ++i; continue; }
			unsigned int tagStart = 0, tagSize = 0;
			iterateChunks(d, [&](const char* tag, int len, unsigned int payload)->bool {
				if (std::memcmp(tag, rm.TagName.c_str(), 4) == 0) { tagStart = payload - 8; tagSize = (unsigned int)len + 8; return false; }
				return true;
				});
			if (tagSize > 0 && tagStart + tagSize <= d.size()) {
				unsigned int tailStart = tagStart + tagSize; unsigned int tailSize = d.size() - tailStart;
				if (tailSize > 0) std::memmove(d.data() + tagStart, d.data() + tailStart, tailSize);
				d.resize(d.size() - tagSize); changed = true;
			}
		}
		return changed;
	}

	bool applyTextureFixes(const std::string& filename, std::vector<unsigned char>& d) {
		bool changed = false;
		constexpr unsigned int IMAGE_LEN = 0x100;
		constexpr unsigned int ENTRY_SIZE = 4 + IMAGE_LEN + 4 + 4;

		auto isDigitsOnly = [](const std::string& s)->bool {
			if (s.empty()) return false;
			for (auto& c : s)
			{
				if (!std::isdigit(static_cast<unsigned char>(c))) return false;
			}
			return true;
			};

		struct PackedTex {
			int ReplaceableId;
			char Image[IMAGE_LEN];
			int UnknownZero;
			unsigned int Flags;
		};

		for (unsigned int qi = 0; qi < textures_.size(); qi++) {
			const auto tf = textures_[qi];
			if (tf.filePath != filename) { ++qi; continue; }

			bool handled = false;
			bool texsFound = false;

			iterateChunks(d, [&](const char* tag, int len, unsigned int payload)->bool {
				if (std::memcmp(tag, "TEXS", 4) != 0) return true;
				texsFound = true;

				unsigned int off = payload;
				unsigned int end = payload + (unsigned int)len;
				int idx = 1;

				int targetIdx = tf.TextureID <= 0 ? 1 : tf.TextureID;

				while (off + ENTRY_SIZE <= end) {
					if (!ensureAvailable(d, off, ENTRY_SIZE)) break;
					PackedTex cur;
					std::memcpy(&cur, d.data() + off, ENTRY_SIZE);

					if (idx == targetIdx) {
						if (!tf.NewTexturePath.empty() && !isDigitsOnly(tf.NewTexturePath)) {
							cur.ReplaceableId = 0;
							std::memset(cur.Image, 0, IMAGE_LEN);
							unsigned int copyLen = tf.NewTexturePath.size();
							if (copyLen > IMAGE_LEN - 1) copyLen = IMAGE_LEN - 1;
							std::memcpy(cur.Image, tf.NewTexturePath.data(), copyLen);
							bool hadText = false;
							for (unsigned int z = 0; z < IMAGE_LEN; ++z) if (cur.Image[z] != 0) { hadText = true; break; }
							if (!hadText) cur.Flags = 0;
						}
						else {
							int newId = 0;
							if (!tf.NewTexturePath.empty() && isDigitsOnly(tf.NewTexturePath)) {
								try { newId = std::stoi(tf.NewTexturePath); }
								catch (...) { newId = 0; }
							}
							else {
								newId = std::atoi(tf.NewTexturePath.c_str());
							}
							cur.ReplaceableId = newId;
							std::memset(cur.Image, 0, IMAGE_LEN);
							cur.Flags = 0;
						}
						std::memcpy(d.data() + off, &cur, ENTRY_SIZE);
						changed = true;
						handled = true;
						return false;
					}

					++idx;
					off += ENTRY_SIZE;
				}

				if (!handled) {
					PackedTex nt{};
					nt.ReplaceableId = 0;
					std::memset(nt.Image, 0, IMAGE_LEN);
					nt.UnknownZero = 0;
					nt.Flags = 0;

					if (!tf.NewTexturePath.empty() && !isDigitsOnly(tf.NewTexturePath)) {
						unsigned int copyLen = tf.NewTexturePath.size();
						if (copyLen > IMAGE_LEN - 1) copyLen = IMAGE_LEN - 1;
						std::memcpy(nt.Image, tf.NewTexturePath.data(), copyLen);
						nt.ReplaceableId = 0;
					}
					else {
						int newId = 0;
						if (!tf.NewTexturePath.empty() && isDigitsOnly(tf.NewTexturePath)) {
							try { newId = std::stoi(tf.NewTexturePath); }
							catch (...) { newId = 0; }
						}
						else newId = std::atoi(tf.NewTexturePath.c_str());
						nt.ReplaceableId = newId;
						std::memset(nt.Image, 0, IMAGE_LEN);
					}

					unsigned int sizeFieldPos = payload - 4;
					unsigned int curChunkSize = 0;
					if (!read_u32_le(d, sizeFieldPos, curChunkSize)) return true;
					unsigned int insertPos = payload + curChunkSize;
					if (insertPos > d.size()) return true;

					d.insert(d.begin() + (ptrdiff_t)insertPos, reinterpret_cast<unsigned char*>(&nt), reinterpret_cast<unsigned char*>(&nt) + ENTRY_SIZE);

					unsigned int newChunkSize = curChunkSize + (unsigned int)ENTRY_SIZE;
					write_u32_le(d, sizeFieldPos, newChunkSize);

					changed = true;
					handled = true;
					return false;
				}

				return true;
				});

			if (!texsFound && !handled) {
				PackedTex nt{};
				nt.ReplaceableId = 0;
				std::memset(nt.Image, 0, IMAGE_LEN);
				nt.UnknownZero = 0;
				nt.Flags = 0;
				if (!tf.NewTexturePath.empty() && !isDigitsOnly(tf.NewTexturePath)) {
					unsigned int copyLen = tf.NewTexturePath.size();
					if (copyLen > IMAGE_LEN - 1) copyLen = IMAGE_LEN - 1;
					std::memcpy(nt.Image, tf.NewTexturePath.data(), copyLen);
				}
				else {
					int newId = 0;
					if (!tf.NewTexturePath.empty() && isDigitsOnly(tf.NewTexturePath)) {
						try { newId = std::stoi(tf.NewTexturePath); }
						catch (...) { newId = 0; }
					}
					else newId = std::atoi(tf.NewTexturePath.c_str());
					nt.ReplaceableId = newId;
				}

				const char tag[4] = { 'T','E','X','S' };
				unsigned int chunkSize = (unsigned int)ENTRY_SIZE;
				d.insert(d.end(), tag, tag + 4);
				d.push_back((unsigned char)(chunkSize & 0xFF)); d.push_back((unsigned char)((chunkSize >> 8) & 0xFF));
				d.push_back((unsigned char)((chunkSize >> 16) & 0xFF)); d.push_back((unsigned char)((chunkSize >> 24) & 0xFF));
				d.insert(d.end(), reinterpret_cast<unsigned char*>(&nt), reinterpret_cast<unsigned char*>(&nt) + ENTRY_SIZE);

				changed = true;
			}
		}

		return changed;
	}

	bool applyAppendPatches(const std::string& filename, std::vector<unsigned char>& d) {
		bool changed = false;
		for (unsigned int i = 0; i < patches_.size(); i++) {
			const auto& p = patches_[i];
			if (p.filePath != filename) { ++i; continue; }
			if (!p.patchData.empty()) { d.insert(d.end(), p.patchData.begin(), p.patchData.end()); changed = true; }
		}
		return changed;
	}
};
