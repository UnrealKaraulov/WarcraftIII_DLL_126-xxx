
#include <filesystem>
#include "Main.h"
#include "Storm.h"
#include "RawImageApi.h"
#include "MdxPatcher.h"

static ModelPatcher gModelPatcher{};

u_int64_t GetBufHash(const char* data, size_t data_len)
{
	u_int64_t hash = 0;
	hash = fnv_64_buf((void*)data, (size_t)data_len, FNV1_64_INIT);
	hash = (hash >> 56) ^ (hash & MASK_56);
	return hash;
}


std::vector<ICONMDLCACHE> ICONMDLCACHELIST;
std::vector<FileRedirectStruct> FileRedirectList;
int NeedDumpFilesToDisk = false;
int __stdcall DumpFilesToDisk(int enabled)
{
	MessageBoxA(0, "ВНИМАНИЕ!", " ", 0);
	NeedDumpFilesToDisk = enabled;
	return enabled;
}


int GetFromIconMdlCache(const std::string filename, ICONMDLCACHE& iconhelperout)
{
	size_t filelen = filename.length();
	u_int64_t hash = GetBufHash(filename.c_str(), filelen);
	for (ICONMDLCACHE& ih : ICONMDLCACHELIST)
	{
		if (ih.hashlen == filelen && ih._hash == hash)
		{
			iconhelperout = ih;
			return true;
		}
	}
	return false;
}

int IsFileRedirected(const std::string filename)
{
	for (FileRedirectStruct& DotaRedirectHelp : FileRedirectList)
	{
		if (filename == DotaRedirectHelp.NewFilePath)
		{
			return true;
		}
	}
	return false;
}

int IsMemInCache(unsigned char* addr)
{
	auto is_even = [&](ICONMDLCACHE s) { return s.buf == addr; };
	auto leakfound = std::find_if(ICONMDLCACHELIST.begin(), ICONMDLCACHELIST.end(), is_even);
	return leakfound != ICONMDLCACHELIST.end();
}

void FreeAllIHelpers()
{
	if (!ICONMDLCACHELIST.empty())
	{
		for (ICONMDLCACHE& ih : ICONMDLCACHELIST)
		{
			if (ih.buf)
				Storm::MemFree(ih.buf);
		}
		ICONMDLCACHELIST.clear();
	}
	if (!FileRedirectList.empty())
		FileRedirectList.clear();
	if (!FakeFileList.empty())
		FakeFileList.clear();
	gModelPatcher.clearAll();

	ClearAllRawImages();
	Storm::ClearAllLeaks();

}


int replaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty() || str.empty()) return 0;
	int replaced = 0;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
		++replaced;
	}
	return replaced;
}


int __stdcall FileHelperReleaseStorm(int enabled)
{
	//NeedReleaseUnusedMemory = enabled;
	return enabled;
}

GameGetFile GameGetFile_org = NULL;
GameGetFile GameGetFile_ptr;

void ApplyTerrainFilter(std::string filename, unsigned char** OutDataPointer, size_t* OutSize, int IsTga)
{
	unsigned char* originfiledata = *OutDataPointer;
	size_t sz = *OutSize;


	int w = 0, h = 0, bpp = 0, mipmaps = 0, alphaflag = 8, compress = 1, alphaenconding = 5;
	unsigned long rawImageSize = 0;
	StormBuffer InBuffer;
	InBuffer.buf = originfiledata;
	InBuffer.length = sz;
	StormBuffer OutBuffer;
	if (IsTga)
		rawImageSize = (unsigned long)TGA2Raw(InBuffer, OutBuffer, w, h, bpp);
	else
		rawImageSize = Blp2Raw(InBuffer, OutBuffer, w, h, bpp, mipmaps, alphaflag, compress, alphaenconding);
	if (rawImageSize > 0)
	{
		COLOR4* OutImage = (COLOR4*)OutBuffer.buf;
		for (unsigned long i = 0; i < OutBuffer.length / 4; i++)
		{
			if ( /*OutImage[ i ].A == 0xFF && */(OutImage[i].G > 40 || OutImage[i].B > 40 || OutImage[i].R > 40))
			{
				if (OutImage[i].R < 235 && OutImage[i].G < 235 && OutImage[i].B < 235)
				{
					OutImage[i].R += 25;
					OutImage[i].G += 25;
					OutImage[i].B += 25;
				}
				OutImage[i].R = 0;
				OutImage[i].G = 0;
				OutImage[i].B = 0;
			}
			else if (true /*OutImage[ i ].A == 0xFF*/)
			{
				if (OutImage[i].R > 0 && OutImage[i].R < 250)
					OutImage[i].R += 5;
				if (OutImage[i].G > 0 && OutImage[i].G < 250)
					OutImage[i].G += 5;
				if (OutImage[i].B > 0 && OutImage[i].B < 250)
					OutImage[i].B += 5;

				OutImage[i].R = 170;
				OutImage[i].G = 170;
				OutImage[i].B = 170;
				OutImage[i].R = 0;
				OutImage[i].G = 0;
				OutImage[i].B = 0;
			}
		}


		StormBuffer ResultBuffer;

		CreatePalettedBLP(OutBuffer, ResultBuffer, 256, filename.c_str(), w, h, bpp, mipmaps);

		if (OutBuffer.buf != NULL)
		{
			OutBuffer.NeedClear = true;
			OutBuffer.Clear();
		}

		if (ResultBuffer.buf != NULL)
		{
			ICONMDLCACHE tmpih;
			tmpih.buf = ResultBuffer.buf;
			tmpih.size = ResultBuffer.length;
			tmpih.hashlen = filename.length();
			tmpih._hash = GetBufHash(filename.c_str(), tmpih.hashlen);
			ICONMDLCACHELIST.push_back(tmpih);

			/*if (!IsMemInCache(*OutDataPointer))
				Storm::MemFree((void*)*OutDataPointer);*/

			*OutDataPointer = tmpih.buf;
			*OutSize = tmpih.size;
		}
	}

}



int __stdcall ApplyTerrainFilterDirectly(char* filename, unsigned char** OutDataPointer, size_t* OutSize, int IsTga)
{
	ApplyTerrainFilter(filename, OutDataPointer, OutSize, IsTga);
	return 0;
}


void ApplyIconFilter(std::string filename, unsigned char** OutDataPointer, size_t* OutSize)
{
	unsigned char* originfiledata = *OutDataPointer;
	size_t sz = *OutSize;


	int w = 0, h = 0, bpp = 0, mipmaps = 0, alphaflag = 0, compress = 0, alphaenconding = 0;
	unsigned long rawImageSize = 0;
	StormBuffer InBuffer;
	InBuffer.buf = originfiledata;
	InBuffer.length = sz;
	StormBuffer OutBuffer;
	rawImageSize = Blp2Raw(InBuffer, OutBuffer, w, h, bpp, mipmaps, alphaflag, compress, alphaenconding);

	if (rawImageSize > 0 && OutBuffer.buf && OutBuffer.length && w == 64 && h == 64)
	{
		//MessageBoxA( 0, "CreateFilterImage", " ", 0 );
		COLOR4* OutImage = (COLOR4*)OutBuffer.buf;
		COLOR4 BlackPix;

		BlackPix.A = 0xFF;
		BlackPix.R = 0;
		BlackPix.G = 0;
		BlackPix.B = 0;

		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 64; y++)
			{
				OutImage[x * 64 + y] = BlackPix;//верх
				OutImage[y * 64 + x] = BlackPix;//лево
				OutImage[(63 - x) * 64 + y] = BlackPix;//низ
				OutImage[y * 64 + 63 - x] = BlackPix;//право
			}
		}

		for (int x = 4; x < 60; x++)
		{
			for (int y = 4; y < 60; y++)
			{
				int id = x * 64 + y;
				int ave = (std::min(std::min(OutImage[id].R, OutImage[id].G), OutImage[id].B) + std::max(std::max(OutImage[id].R, OutImage[id].G), OutImage[id].B)) / 2;
				OutImage[id].R = FixBounds((ave + OutImage[id].R) / 4);
				OutImage[id].G = FixBounds((ave + OutImage[id].G) / 4);
				OutImage[id].B = FixBounds((ave + OutImage[id].B) / 4);
			}
		}

		//градиентные рамки
		//8 полос градиента
		for (int x = 4; x < 12; x++)
		{
			for (int y = x; y < 64 - x; y++)
			{
				double colorfix = (x - 3.0) / 9.0;

				OutImage[x * 64 + y].R = FixBounds(colorfix * OutImage[x * 64 + y].R);//верх
				OutImage[x * 64 + y].G = FixBounds(colorfix * OutImage[x * 64 + y].G);//верх
				OutImage[x * 64 + y].B = FixBounds(colorfix * OutImage[x * 64 + y].B);//верх

				OutImage[y * 64 + x].R = FixBounds(colorfix * OutImage[y * 64 + x].R);//лево
				OutImage[y * 64 + x].G = FixBounds(colorfix * OutImage[y * 64 + x].G);//лево
				OutImage[y * 64 + x].B = FixBounds(colorfix * OutImage[y * 64 + x].B);//лево

				OutImage[(63 - x) * 64 + y].R = FixBounds(colorfix * OutImage[(63 - x) * 64 + y].R);//низ
				OutImage[(63 - x) * 64 + y].G = FixBounds(colorfix * OutImage[(63 - x) * 64 + y].G);//низ
				OutImage[(63 - x) * 64 + y].B = FixBounds(colorfix * OutImage[(63 - x) * 64 + y].B);//низ

				OutImage[y * 64 + 63 - x].R = FixBounds(colorfix * OutImage[y * 64 + 63 - x].R);//право
				OutImage[y * 64 + 63 - x].G = FixBounds(colorfix * OutImage[y * 64 + 63 - x].G);//право
				OutImage[y * 64 + 63 - x].B = FixBounds(colorfix * OutImage[y * 64 + 63 - x].B);//право
			}
		}

		StormBuffer ResultBuffer;
		CreatePalettedBLP(OutBuffer, ResultBuffer, 256, filename.c_str(), w, h, bpp, mipmaps);

		OutBuffer.Clear();
		if (ResultBuffer.buf != NULL)
		{
			ICONMDLCACHE tmpih;
			tmpih.buf = ResultBuffer.buf;
			tmpih.size = ResultBuffer.length;
			tmpih.hashlen = filename.length();
			tmpih._hash = GetBufHash(filename.c_str(), tmpih.hashlen);
			ICONMDLCACHELIST.push_back(tmpih);

			/*if (!IsMemInCache(*OutDataPointer))
				Storm::MemFree((void*)*OutDataPointer);*/

			*OutDataPointer = tmpih.buf;
			*OutSize = tmpih.size;

			if (NeedDumpFilesToDisk)
			{
				//fs::create_directories( )

				try
				{

					fs::path p = "DotaAllstars\\" + filename;
					fs::path dir = p.parent_path();

					if (dir.string().length() > 0)
					{
						std::error_code err;
						fs::create_directories(dir.string(), err);
					}

					FILE* f;
					fopen_s(&f, ("DotaAllstars\\" + filename).c_str(), "wb");
					if (f)
					{
						fwrite(tmpih.buf, tmpih.size, 1, f);

						fclose(f);
					}

				}
				catch (...)
				{

				}
			}
		}
	}
}


void ApplyIconFrameFilter(std::string filename);


void ApplyTestFilter(std::string filename, unsigned char** OutDataPointer, size_t* OutSize)
{

	ICONMDLCACHE tmpih;


	unsigned char* originfiledata = *OutDataPointer;
	size_t sz = *OutSize;


	int w = 0, h = 0, bpp = 0, mipmaps = 0, alphaflag = 0, compress = 0, alphaenconding = 0;
	unsigned long rawImageSize = 0;
	StormBuffer InBuffer;
	InBuffer.buf = originfiledata;
	InBuffer.length = sz;
	StormBuffer OutBuffer;

	rawImageSize = Blp2Raw(InBuffer, OutBuffer, w, h, bpp, mipmaps, alphaflag, compress, alphaenconding);
	if (rawImageSize > 0 && w > 9 && h > 9)
	{
		COLOR4* OutImage = (COLOR4*)OutBuffer.buf;
		COLOR4 BlackPix;

		BlackPix.A = 0xFF;
		BlackPix.R = 70;
		BlackPix.G = 70;
		BlackPix.B = 70;

		//int FoundTransparentTexture = false;

		int id = 0;
		std::vector<COLOR4> BGRAPixList;
		BGRAPixList.assign(&OutImage[0], &OutImage[w * h - 1]);


		for (COLOR4& pix : BGRAPixList)
		{
			int R = pix.R;
			int G = pix.G;
			int B = pix.B;

			//pix.G = FixBounds( ( max( max( pix.R, pix.G ), pix.B ) + min( min( pix.R, pix.G ), pix.B ) ) / 2 );
			//pix.B = FixBounds( ( max( max( pix.R, pix.G ), pix.B ) + min( min( pix.R, pix.G ), pix.B ) ) / 2 );
			//pix.R = FixBounds( ( max( max( pix.R, pix.G), pix.B ) + min( min( pix.R, pix.G), pix.B ) ) / 2 );

			pix.R = FixBounds(0.2126 * R + 0.7152 * G + 0.0722 * B);
			pix.G = FixBounds(0.2126 * R + 0.7152 * G + 0.0722 * B);
			pix.B = FixBounds(0.2126 * R + 0.7152 * G + 0.0722 * B);

			/*pix.R = FixBounds( ( pix.R * .393 ) + ( pix.G *.769 ) + ( pix.B * .189 ) );
			pix.G = FixBounds( ( pix.R * .349 ) + ( pix.G *.686 ) + ( pix.B * .168 ) );
			pix.B = FixBounds( ( pix.R * .272 ) + ( pix.G *.534 ) + ( pix.B * .131 ) );
			*/


			id++;
		}

		std::memcpy(&OutImage[0], &BGRAPixList[0], 4 * w * h - 4);

		/*

		for ( int x = 0; x < h; x++ )
		{
			for ( int y = 0; y < w; x++ )
			{
				BlackPix.A = OutImage[ x * h + y ].A;
				if ( BlackPix.A < 0xFF )
				{
					FoundTransparentTexture = true;
					break;
				}
			}
		}


		if ( !FoundTransparentTexture )
			std::fill( &OutImage[ 0 ], &OutImage[ h * w - 1 ], BlackPix );

		BlackPix.R = 0;
		BlackPix.G = 0;
		BlackPix.B = 0;
		*/
		//
		//for ( int x = 0; x < h; x++ )
		//{
		//	for ( int y = 0; y < w; x++ )
		//	{
		//		BlackPix.A = OutImage[ x * h + y ].A;
		//		OutImage[ x * h + y ] = BlackPix;//верх
		//	}
		//}

		StormBuffer ResultBuffer;

		CreatePalettedBLP(OutBuffer, ResultBuffer, 256, filename.c_str(), w, h, bpp, mipmaps);

		if (OutBuffer.buf != NULL)
		{
			OutBuffer.length = 0;
			Storm::MemFree(OutBuffer.buf);
			OutBuffer.buf = 0;
		}

		if (ResultBuffer.buf != NULL)
		{
			tmpih.buf = ResultBuffer.buf;
			tmpih.size = ResultBuffer.length;
			tmpih.hashlen = filename.length();
			tmpih._hash = GetBufHash(filename.c_str(), tmpih.hashlen);
			ICONMDLCACHELIST.push_back(tmpih);

			/*if (!IsMemInCache(*OutDataPointer))
				Storm::MemFree((void*)*OutDataPointer);*/

			*OutDataPointer = tmpih.buf;
			*OutSize = tmpih.size;
		}
	}

}


const char* DisabledIconSignature = "Disabled\\DIS";
const char* DisabledIconSignature2 = "Disabled\\DISDIS";
const char* CommandButtonsDisabledIconSignature = "CommandButtonsDisabled\\DIS";


int FixDisabledIconPath(std::string _filename, unsigned char** OutDataPointer, size_t* OutSize, int unknown)
{
	std::string filename = _filename;

	//int CreateDarkIcon = false;
	int result = false;


	if (filename.find(DisabledIconSignature2) != std::string::npos)
	{
		if (replaceAll(filename, DisabledIconSignature2, "\\"))
		{
			result = GameGetFile_ptr(filename.c_str(), OutDataPointer, OutSize, unknown);
		}
	}


	if (!result)
	{
		filename = _filename;
		if (filename.find(DisabledIconSignature) != std::string::npos)
		{
			if (replaceAll(filename, DisabledIconSignature, "\\"))
			{
				result = GameGetFile_ptr(filename.c_str(), OutDataPointer, OutSize, unknown);
			}
		}
	}

	if (!result)
	{
		filename = _filename;
		if (filename.find(DisabledIconSignature) != std::string::npos)
		{
			if (replaceAll(filename, CommandButtonsDisabledIconSignature, "PassiveButtons\\"))
			{
				result = GameGetFile_ptr(filename.c_str(), OutDataPointer, OutSize, unknown);
			}
		}
	}


	if (!result)
	{
		filename = _filename;
		if (filename.find(DisabledIconSignature) != std::string::npos)
		{
			if (replaceAll(filename, CommandButtonsDisabledIconSignature, "AutoCastButtons\\"))
			{
				result = GameGetFile_ptr(filename.c_str(), OutDataPointer, OutSize, unknown);
			}
		}
	}


	if (result)
	{
		ApplyIconFilter(_filename, OutDataPointer, OutSize);
	}
	//else MessageBoxA( 0, filename, "Bad file path:", 0 );

	return result;
}
int __stdcall FixModelCollisionSphere(const char* mdlpath, float X, float Y, float Z, float Radius)
{
	if (!mdlpath) return 0;
	gModelPatcher.setCollisionSphere(mdlpath, 999, X, Y, Z, Radius);
	return 1;
}

int __stdcall FixModelCollisionSphereEx(const char* mdlpath, int collision_id, float X, float Y, float Z, float Radius)
{
	if (!mdlpath) return 0;
	gModelPatcher.setCollisionSphere(mdlpath, collision_id, X, Y, Z, Radius);
	return 1;
}

int __stdcall FixModelCollisionBox(const char* mdlpath, int collision_id, float X, float Y, float Z, float Radius)
{
	if (!mdlpath) return 0;
	gModelPatcher.setCollisionBox(mdlpath, collision_id, X, Y, Z, Radius, Radius, Radius);
	return 1;
}

int __stdcall FixModelCollisionBoxEx(const char* mdlpath, int collision_id, float X, float Y, float Z, float halfX, float  halfY, float halfZ)
{
	if (!mdlpath) return 0;
	gModelPatcher.setCollisionBox(mdlpath, collision_id, X, Y, Z, halfX, halfY, halfZ);
	return 1;
}


// texturenew texture name or replaceable id ("test\image.blp" or "5")
int __stdcall FixModelTexturePath(const char* mdlpath, int textureid, const char* texturenew)
{
	if (!mdlpath || !texturenew) return 0;
	gModelPatcher.fixTexture(mdlpath, textureid, texturenew);
	return 1;
}

int __stdcall PatchModel(const char* mdlpath, const char* pathPatch)
{
	if (!mdlpath || !pathPatch) return 0;
	unsigned char* patchPtr = nullptr;
	size_t patchSz = 0;
	// GameGetFile_ptr обычно возвращает int и принимает (const char*, unsigned char**, size_t*, int)
	int ok = GameGetFile_ptr(pathPatch, &patchPtr, &patchSz, 0);
	if (ok && patchPtr && patchSz) {
		std::vector<unsigned char> patchData;
		patchData.assign(patchPtr, patchPtr + patchSz);
		gModelPatcher.addPatch(mdlpath, patchData);
		// не освобождаем patchPtr здесь?
		return 1;
	}
	return 0;
}

int __stdcall RemoveTagFromModel(const char* mdlpath, const char* tagname)
{
	if (!mdlpath || !tagname) return 0;
	gModelPatcher.removeTag(mdlpath, tagname);
	return 1;
}
//  sequence name or id ("Attack - 2" or "5")
int __stdcall ChangeAnimationSpeed(const char* mdlpath, const char* SeqenceName, float Speed)
{
	if (!mdlpath || !SeqenceName) return 0;
	gModelPatcher.changeAnimationSpeed(mdlpath, SeqenceName, Speed);
	return 1;
}

// sequence name
int __stdcall SetSequenceValue(const char* mdlpath, const char* SeqenceName, int Indx, float Value)
{
	if (!mdlpath) return 0;
	if (Indx < 0 || Indx > 6) return 0;
	gModelPatcher.setSequenceValue(mdlpath, SeqenceName ? SeqenceName : "", Indx, Value);
	return 1;
}

int __stdcall SetModelScale(const char* mdlpath, float Scale)
{
	if (!mdlpath) return 0;
	gModelPatcher.setModelScale(mdlpath, Scale);
	return 1;
}

int __stdcall SetModelScaleEx(const char* mdlpath, float x, float y, float z)
{
	if (!mdlpath) return 0;
	gModelPatcher.setModelScaleEx(mdlpath, x, y, z);
	return 1;
}


std::vector<unsigned char> FullPatchData;


void ProcessMdx(std::string filename, unsigned char** OutDataPointer, size_t* OutSize)
{
	if (!OutDataPointer || !OutSize || !*OutDataPointer || *OutSize < 8)
		return;

	// Проверить кэш перед обработкой
	ICONMDLCACHE tmpih;
	int FoundOldHelper = GetFromIconMdlCache(filename, tmpih);
	if (FoundOldHelper) {
		*OutDataPointer = tmpih.buf;
		*OutSize = tmpih.size;
		return;
	}

	std::vector<unsigned char> buf(*OutDataPointer, *OutDataPointer + *OutSize);
	bool changed = gModelPatcher.processModel(filename, buf);

	if (changed) {
		unsigned char* newMem = (unsigned char*)Storm::MemAlloc(buf.size());
		if (newMem) {
			std::memcpy(newMem, buf.data(), buf.size());

			// Добавить в кэш
			ICONMDLCACHE newCache;
			newCache.buf = newMem;
			newCache.size = buf.size();
			newCache.hashlen = filename.length();
			newCache._hash = GetBufHash(filename.c_str(), newCache.hashlen);
			ICONMDLCACHELIST.push_back(newCache);

			*OutDataPointer = newMem;
			*OutSize = buf.size();
		}
	}


	gModelPatcher.removeProcessedTasks(filename);
}


int __stdcall RedirectFile(const char* RealFilePath, const char* NewFilePath)
{
	FileRedirectStruct tmpModelFix;
	tmpModelFix.NewFilePath = NewFilePath;
	tmpModelFix.RealFilePath = RealFilePath;
	FileRedirectList.push_back(tmpModelFix);
	return 0;
}

int ProcessFile(const char* filename, unsigned char** OutDataPointer, size_t* OutSize, int unknown, int IsFileExistOld)
{
	int IsFileExist = IsFileExistOld;

	if (!OutDataPointer || !OutSize || !filename)
	{
		return IsFileExist;
	}

	ICONMDLCACHE tmpih;
	int FoundOldHelper = GetFromIconMdlCache(filename, tmpih);
	if (FoundOldHelper)
	{
		*OutDataPointer = tmpih.buf;
		*OutSize = tmpih.size;
		return true;
	}

	for (RawImageStruct& s : ListOfRawImages)
	{
		if (s.ingame)
		{
			if (_stricmp(filename, s.filename) == 0)
			{
				*OutDataPointer = s.ingamebuffer.buf;
				*OutSize = s.ingamebuffer.length;
				return true;
			}
		}
	}



	std::string FileExtension = "";

	try
	{
		FileExtension = ToLower(fs::path(filename).extension().string());
	}
	catch (...)
	{

	}


	if (FileExtension == std::string(".tga"))
	{

	}
	else if (FileExtension == std::string(".blp"))
	{
		if (!IsFileExist)
		{
			IsFileExist = FixDisabledIconPath(filename, OutDataPointer, OutSize, unknown);
		}
		else
		{
			//ApplyTestFilter( filename, OutDataPointer, OutSize );
			/*if ( strstr( filename.c_str( ), "terrainart" ) == filename.c_str( ) ||
				strstr( filename.c_str( ), "replaceabletextures\\cliff" ) == filename.c_str( ) )
				ApplyTerrainFilter( filename, OutDataPointer, OutSize, false );*/
		}
	}
	else if (FileExtension == std::string(".mdx"))
	{
		if (IsFileExist)
		{
			ProcessMdx(filename, OutDataPointer, OutSize);
		}
		else
		{
			//return GameGetFile_ptr( "Objects\\InvalidObject\\InvalidObject.mdx", OutDataPointer, OutSize, unknown );
		}
	}


	for (FileRedirectStruct& DotaRedirectHelp : FileRedirectList)
	{
		if (filename == DotaRedirectHelp.NewFilePath)
		{
			ICONMDLCACHE tmpih2 = ICONMDLCACHE();

			IsFileExist = GameGetFile_ptr(DotaRedirectHelp.RealFilePath.c_str(), OutDataPointer, OutSize, unknown);
			if (IsFileExist)
			{
				unsigned char* DataPointer = *OutDataPointer;
				size_t DataSize = *OutSize;

				StormBuffer ResultBuffer;
				ResultBuffer.buf = (unsigned char*)Storm::MemAlloc(DataSize);
				ResultBuffer.length = DataSize;
				std::memcpy(&ResultBuffer.buf[0], DataPointer, DataSize);

				tmpih2.buf = ResultBuffer.buf;
				tmpih2.size = ResultBuffer.length;

				tmpih2.hashlen = DotaRedirectHelp.NewFilePath.length();
				tmpih2._hash = GetBufHash(DotaRedirectHelp.NewFilePath.c_str(), tmpih2.hashlen);

				ICONMDLCACHELIST.push_back(tmpih2);

				*OutDataPointer = tmpih2.buf;
				*OutSize = tmpih2.size;


				if (FileExtension == std::string(".tga"))
				{

				}
				else if (FileExtension == std::string(".blp"))
				{
					if (!IsFileExist)
					{
						IsFileExist = FixDisabledIconPath(filename, OutDataPointer, OutSize, unknown);
					}
					else
					{
						//ApplyTestFilter( filename, OutDataPointer, OutSize );
						/*if ( strstr( filename.c_str( ), "terrainart" ) == filename.c_str( ) ||
						strstr( filename.c_str( ), "replaceabletextures\\cliff" ) == filename.c_str( ) )
						ApplyTerrainFilter( filename, OutDataPointer, OutSize, false );*/
					}
				}
				else if (FileExtension == std::string(".mdx"))
				{
					if (IsFileExist)
					{
						ProcessMdx(filename, OutDataPointer, OutSize);
					}
					else
					{
						//return GameGetFile_ptr( "Objects\\InvalidObject\\InvalidObject.mdx", OutDataPointer, OutSize, unknown );
					}
				}



				//ProcessFile( DotaRedirectHelp.NewFilePath, OutDataPointer, OutSize, unknown, IsFileExist );



				return true;
			}

		}
	}

	if (!IsFileExist && IsFileExistOld)
		return IsFileExistOld;

	return IsFileExist;
}

std::vector<FakeFileStruct> FakeFileList;

void AddNewFakeFile(char* filename, unsigned char* buffer, size_t FileSize)
{
	FakeFileStruct tmpstr;
	tmpstr.buffer = buffer;
	tmpstr.filename = filename;
	tmpstr.size = FileSize;
	FakeFileList.push_back(tmpstr);
}

//
//SFileOpenFile SFileOpenFile_org, SFileOpenFile_ptr;
//SFileReadFile SFileReadFile_org, SFileReadFile_ptr;
//
//
//
//int __stdcall SFileOpenFile_my( const char* filename, int * handle )
//{
//
//	for ( RawImageStruct & s : ListOfRawImages )
//	{
//		if ( s.ingame )
//		{
//			if ( ToLower( filename ) == ToLower( s.filename ) )
//			{
//				*handle = -s.RawImage;
//				return true;
//			}
//		}
//	}
//
//	return  SFileOpenFile_ptr( filename, handle );
//}
//
//
//int __stdcall SFileReadFile_my( int file_handle, void* buffer, unsigned int size, unsigned int* size_ptr, unsigned int unk )
//{
//
//	for ( RawImageStruct & s : ListOfRawImages )
//	{
//		if ( s.ingame )
//		{
//			if ( file_handle == -s.RawImage )
//			{
//				if ( !buffer )
//					return false;
//
//				if ( size <= s.ingamebuffer.length )
//				{
//					std::memcpy( buffer, s.ingamebuffer.buf, size );
//
//					if ( size_ptr )
//						*size_ptr = size;
//					return true;
//				}
//
//				if ( size_ptr )
//					*size_ptr = 0;
//				return false;
//			}
//		}
//	}
//
//	return SFileReadFile_ptr( file_handle, buffer, size, size_ptr, unk );
//}

int __fastcall GameGetFile_my(const char* filename, unsigned char** OutDataPointer, unsigned int* OutSize, int unknown)
{
	int IsFileExist = GameGetFile_ptr(filename, OutDataPointer, OutSize, unknown);


	// ONLY FOR TEST
	//if ( filename && filename[ 0 ] != '\0' )
	//{
	//	std::ofstream outfile;
	//	outfile.open( "test.txt", std::ios_base::app );
	//	outfile << filename << std::endl;
	//	outfile.close( );
	//}

	if (!(IsGame()) && !MainFuncWork)
	{
		return IsFileExist;
	}

	if (!OutDataPointer || !OutSize)
	{
		return GameGetFile_ptr(filename, OutDataPointer, OutSize, unknown);
	}


	if (filename == NULL || *filename == '\0')
	{
		return IsFileExist;
	}

	for (FakeFileStruct& fs : FakeFileList)
	{
		if (_stricmp(filename, fs.filename) == 0)
		{
			*OutDataPointer = fs.buffer;
			*OutSize = fs.size;
			return true;
		}
	}

	IsFileExist = ProcessFile(filename, OutDataPointer, OutSize, unknown, IsFileExist);

	return IsFileExist;
}



//iconpath + _frame.blp
int __stdcall CreateIconFrameMask(const char* iconpath)
{
	ApplyIconFrameFilter(iconpath);


	return true;
}



//Storm_279 Storm_279_org;
//Storm_279 Storm_279_ptr;
//int __stdcall Storm_279_my( const char * filename, int arg1, int arg2, size_t arg3, int arg4 )
//{
//
//	if ( ConfigWindowCreated )
//	{
//		if ( ToLower( "UI\\FrameDef\\Glue\\DialogWar3.fdf" ) == ToLower( filename ) )
//		{
//			return Storm_279_ptr( "DotaConfigEditor.fdf", arg1, arg2, arg3, arg4 );
//		}
//	}
//
//	return Storm_279_ptr( filename, arg1, arg2, arg3, arg4 );
//}



/*

	Draw images

*/

std::string GetFileContent(const std::string& filename)
{
	std::ifstream t(filename, std::ios::binary);
	if (!t) return {};
	t.seekg(0, std::ios::end);
	std::streampos end = t.tellg();
	if (end <= 0) return {};
	std::string str;
	str.resize(static_cast<size_t>(end));
	t.seekg(0, std::ios::beg);
	t.read(&str[0], end);
	return str;
}


std::vector<std::string> get_file_list(const fs::path& path, bool dotolower)
{
	std::vector<std::string> file_list;

	if (path.empty() || !fs::exists(path)) {
		return file_list;
	}

	std::error_code err;
	auto options = fs::directory_options::skip_permission_denied;

	try {
		fs::recursive_directory_iterator it(path, options, err);
		fs::recursive_directory_iterator end;

		if (err) {
			return file_list;
		}

		for (; it != end; it.increment(err)) {
			if (err) {
				continue;
			}

			// Фильтруем только файлы если нужно
			if (!fs::is_regular_file(it->path())) {
				continue;
			}

			const auto& current_path = it->path();

			if (dotolower) {
				file_list.push_back(ToLower(current_path.string()));
			}
			else {
				file_list.push_back(current_path.string());
			}
		}
	}
	catch (...) 
	{

	}

	return file_list;
}


typedef void(__fastcall* EnableLocalFiles_p)(int enable);
EnableLocalFiles_p EnableLocalFiles_org = NULL;


int __stdcall IsLocalFilesEnabled(int)
{
	if (!GameDll) return 0;
	return *(int*)(GameDll + 0xAAE2AC);
}

void __stdcall EnableLocalFiles(int enable)
{
	if (!GameDll) return;
	EnableLocalFiles_org = (EnableLocalFiles_p)(GameDll + 0x0010B0);
	if (EnableLocalFiles_org) EnableLocalFiles_org(enable);
}

int __stdcall CheckWriteAccess(int)
{
	FILE* f = NULL;
	fopen_s(&f, "CHECKWRITEACCESS.TGA", "wb");
	if (!f)
	{
		return false;
	}
	if (!fprintf_s(f, "%s", "test") || ferror(f))
	{
		fclose(f);
		DeleteFileA("CHECKWRITEACCESS.TGA");
		return false;
	}
	fclose(f);
	DeleteFileA("CHECKWRITEACCESS.TGA");
	return true;
}