
#include <filesystem>
#include "Main.h"
#include "Storm.h"
#include "RawImageApi.h"


u_int64_t GetBufHash(const char* data, size_t data_len)
{
	u_int64_t hash;
	hash = fnv_64_buf((void*)data, (size_t)data_len, FNV1_64_INIT);
	hash = (hash >> 56) ^ (hash & MASK_56);
	return hash;
}


std::vector<ICONMDLCACHE> ICONMDLCACHELIST;
std::vector<FileRedirectStruct> FileRedirectList;
int NeedDumpFilesToDisk = false;
int __stdcall DumpFilesToDisk(int enabled)
{
	MessageBoxA(0, "ќЎ»Ѕ ј OSHIBKA ERROR", " ", 0);
	//NeedDumpFilesToDisk = enabled;
	return enabled;
}


int GetFromIconMdlCache(const std::string filename, ICONMDLCACHE* iconhelperout)
{
	size_t filelen = filename.length();
	u_int64_t hash = GetBufHash(filename.c_str(), filelen);
	for (ICONMDLCACHE& ih : ICONMDLCACHELIST)
	{
		if (ih.hashlen == filelen && ih._hash == hash)
		{
			*iconhelperout = ih;
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
	for (ICONMDLCACHE& ih : ICONMDLCACHELIST)
	{
		if ((unsigned char*)ih.buf == addr)
			return true;
	}
	return false;
}

void FreeAllIHelpers()
{
	if (!ICONMDLCACHELIST.empty())
	{
		for (ICONMDLCACHE& ih : ICONMDLCACHELIST)
		{
			if (ih.buf && NeedReleaseUnusedMemory)
				Storm::MemFree(ih.buf);
		}


		Storm::FreeAllMemory();

		ICONMDLCACHELIST.clear();
	}
	if (!FileRedirectList.empty())
		FileRedirectList.clear();

	if (!FakeFileList.empty())
		FakeFileList.clear();
	ClearAllRawImages();

}


char MPQFilePath[4000];



int replaceAll(std::string& str, const std::string& from, const std::string& to)
{
	int Replaced = false;
	if (from.empty())
		return Replaced;
	if (str.empty())
		return Replaced;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
		Replaced = true;
	}
	return Replaced;
}

int NeedReleaseUnusedMemory = false;

int __stdcall FileHelperReleaseStorm(int enabled)
{
	NeedReleaseUnusedMemory = enabled;
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
		rawImageSize = (unsigned long)TGA2Raw(InBuffer, OutBuffer, w, h, bpp, filename.c_str());
	else
		rawImageSize = Blp2Raw(InBuffer, OutBuffer, w, h, bpp, mipmaps, alphaflag, compress, alphaenconding, filename.c_str());
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

		CreatePalettedBLP(OutBuffer, ResultBuffer, 256, filename.c_str(), w, h, bpp, alphaflag, mipmaps);

		if (OutBuffer.buf != NULL)
		{
			OutBuffer.length = 0;
			Storm::MemFree(OutBuffer.buf);
			OutBuffer.buf = 0;
		}

		if (ResultBuffer.buf != NULL)
		{
			ICONMDLCACHE tmpih;
			tmpih.buf = ResultBuffer.buf;
			tmpih.size = ResultBuffer.length;
			tmpih.hashlen = filename.length();
			tmpih._hash = GetBufHash(filename.c_str(), tmpih.hashlen);
			ICONMDLCACHELIST.push_back(tmpih);/*
			if ( !IsMemInCache( *OutDataPointer ) && NeedReleaseUnusedMemory )
				Storm::MemFree( ( void* )*OutDataPointer );*/
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


void ApplyIconFilter(std::string filename, unsigned char ** OutDataPointer, size_t* OutSize)
{
	unsigned char* originfiledata = *OutDataPointer;
	size_t sz = *OutSize;


	int w = 0, h = 0, bpp = 0, mipmaps = 0, alphaflag = 0, compress = 0, alphaenconding = 0;
	unsigned long rawImageSize = 0;
	StormBuffer InBuffer;
	InBuffer.buf = originfiledata;
	InBuffer.length = sz;
	StormBuffer OutBuffer;
	rawImageSize = Blp2Raw(InBuffer, OutBuffer, w, h, bpp, mipmaps, alphaflag, compress, alphaenconding, filename.c_str());

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
				int ave = (min(min(OutImage[id].R, OutImage[id].G), OutImage[id].B) + max(max(OutImage[id].R, OutImage[id].G), OutImage[id].B)) / 2;
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
		CreatePalettedBLP(OutBuffer, ResultBuffer, 256, filename.c_str(), w, h, bpp, alphaflag, mipmaps);

		OutBuffer.Clear();
		if (ResultBuffer.buf != NULL)
		{
			ICONMDLCACHE tmpih;
			tmpih.buf = ResultBuffer.buf;
			tmpih.size = ResultBuffer.length;
			tmpih.hashlen = filename.length();
			tmpih._hash = GetBufHash(filename.c_str(), tmpih.hashlen);
			ICONMDLCACHELIST.push_back(tmpih);
			/*if ( !IsMemInCache( *OutDataPointer ) && NeedReleaseUnusedMemory )
				Storm::MemFree( ( void* )*OutDataPointer );*/
			*OutDataPointer = tmpih.buf;
			*OutSize = tmpih.size;

			if (NeedDumpFilesToDisk)
			{
				//fs::create_directories( )

				fs::path p("DotaAllstars\\" + filename);
				fs::path dir = p.parent_path();

				if (dir.string().length() > 0)
				{
					fs::create_directories(dir.string());
				}

				FILE* f;
				fopen_s(&f, ("DotaAllstars\\" + filename).c_str(), "wb");
				if (f)
				{
					fwrite(tmpih.buf, tmpih.size, 1, f);

					fclose(f);
				}

			}
		}
	}
}


void ApplyIconFrameFilter(std::string filename, int* OutDataPointer, size_t* OutSize);


void ApplyTestFilter(std::string filename, unsigned char ** OutDataPointer, size_t* OutSize)
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

	rawImageSize = Blp2Raw(InBuffer, OutBuffer, w, h, bpp, mipmaps, alphaflag, compress, alphaenconding, filename.c_str());
	if (rawImageSize > 0 && w > 9 && h > 9)
	{
		COLOR4* OutImage = (COLOR4*)OutBuffer.buf;
		COLOR4 BlackPix;

		BlackPix.A = 0xFF;
		BlackPix.R = 70;
		BlackPix.G = 70;
		BlackPix.B = 70;

		int FoundTransparentTexture = false;

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

		CreatePalettedBLP(OutBuffer, ResultBuffer, 256, filename.c_str(), w, h, bpp, alphaflag, mipmaps);

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
			/*	if ( !IsMemInCache( *OutDataPointer ) && NeedReleaseUnusedMemory )
					Storm::MemFree( ( void* )*OutDataPointer );*/
			*OutDataPointer = tmpih.buf;
			*OutSize = tmpih.size;
		}
	}

}


const char* DisabledIconSignature = "Disabled\\DIS";
const char* DisabledIconSignature2 = "Disabled\\DISDIS";
const char* CommandButtonsDisabledIconSignature = "CommandButtonsDisabled\\DIS";


int FixDisabledIconPath(std::string _filename, unsigned char ** OutDataPointer, size_t* OutSize, int unknown)
{
	std::string filename = _filename;

	int CreateDarkIcon = false;
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

std::vector<ModelCollisionFixStruct> ModelCollisionFixList;
std::vector<ModelTextureFixStruct> ModelTextureFixList;
std::vector<ModelPatchStruct> ModelPatchList;
std::vector<ModelRemoveTagStruct> ModelRemoveTagList;
std::vector<ModelSequenceReSpeedStruct> ModelSequenceReSpeedList;
std::vector<ModelSequenceValueStruct> ModelSequenceValueList;
std::vector<ModelScaleStruct> ModelScaleList;

int __stdcall FixModelCollisionSphere(const char* mdlpath, float X, float Y, float Z, float Radius)
{
	ModelCollisionFixStruct tmpModelFix;
	tmpModelFix.FilePath = mdlpath;
	tmpModelFix.X = X;
	tmpModelFix.Y = Y;
	tmpModelFix.Z = Z;
	tmpModelFix.Radius = Radius;
	ModelCollisionFixList.push_back(tmpModelFix);
	return 0;
}


int __stdcall FixModelTexturePath(const char* mdlpath, int textureid, const char* texturenew)
{
	ModelTextureFixStruct tmpModelFix;
	tmpModelFix.FilePath = mdlpath;
	tmpModelFix.TextureID = textureid;
	tmpModelFix.NewTexturePath = texturenew;
	ModelTextureFixList.push_back(tmpModelFix);
	return 0;
}



int __stdcall PatchModel(const char* mdlpath, const char* pathPatch)
{
	ModelPatchStruct tmpModelFix;
	tmpModelFix.FilePath = mdlpath;
	tmpModelFix.patchPath = pathPatch;
	ModelPatchList.push_back(tmpModelFix);
	return 0;
}

int __stdcall RemoveTagFromModel(const char* mdlpath, const char* tagname)
{
	ModelRemoveTagStruct tmpModelFix;
	tmpModelFix.FilePath = mdlpath;
	tmpModelFix.TagName = tagname;
	ModelRemoveTagList.push_back(tmpModelFix);
	return 0;
}

int __stdcall ChangeAnimationSpeed(const char* mdlpath, const char* SeqenceName, float Speed)
{
	ModelSequenceReSpeedStruct tmpModelFix;
	tmpModelFix.FilePath = mdlpath;
	tmpModelFix.AnimationName = SeqenceName;
	tmpModelFix.SpeedUp = Speed;
	ModelSequenceReSpeedList.push_back(tmpModelFix);
	return 0;
}



int __stdcall SetSequenceValue(const char* mdlpath, const char* SeqenceName, int Indx, float Value)
{
	if (Indx < 0 || Indx > 6)
		return -1;

	ModelSequenceValueStruct tmpModelFix;
	tmpModelFix.FilePath = mdlpath;
	tmpModelFix.AnimationName = SeqenceName;
	tmpModelFix.Indx = Indx;
	tmpModelFix.Value = Value;
	ModelSequenceValueList.push_back(tmpModelFix);
	return 0;
}


int __stdcall SetModelScale(const char* mdlpath, float Scale)
{
	ModelScaleStruct tmpModelFix;
	tmpModelFix.FilePath = mdlpath;
	tmpModelFix.Scale = Scale;
	tmpModelFix.ScaleX = 0.0f;
	tmpModelFix.ScaleY = 0.0f;
	tmpModelFix.ScaleZ = 0.0f;
	ModelScaleList.push_back(tmpModelFix);
	return 0;
}

int __stdcall SetModelScaleEx(const char* mdlpath, float x, float y, float z)
{
	ModelScaleStruct tmpModelFix;
	tmpModelFix.FilePath = mdlpath;
	tmpModelFix.Scale = 0.0f;
	tmpModelFix.ScaleX = x;
	tmpModelFix.ScaleY = y;
	tmpModelFix.ScaleZ = z;
	ModelScaleList.push_back(tmpModelFix);
	return 0;
}

std::vector<unsigned char> FullPatchData;

void ProcessNodeAnims(unsigned char* ModelBytes, size_t _offset, std::vector<int*>& TimesForReplace)
{
	Mdx_Track tmpTrack;
	size_t offset = _offset;
	if (memcmp(&ModelBytes[offset], "KGTR", 4) == 0)
	{
		offset += 4;
		std::memcpy(&tmpTrack, &ModelBytes[offset], sizeof(Mdx_Track));
		offset += sizeof(Mdx_Track);
		for (int i = 0; i < tmpTrack.NrOfTracks; i++)
		{
			TimesForReplace.push_back((int*)&ModelBytes[offset]);
			offset += (tmpTrack.InterpolationType > 1 ? 40 : 16);
		}
	}

	if (memcmp(&ModelBytes[offset], "KGRT", 4) == 0)
	{
		offset += 4;
		std::memcpy(&tmpTrack, &ModelBytes[offset], sizeof(Mdx_Track));
		offset += sizeof(Mdx_Track);
		for (int i = 0; i < tmpTrack.NrOfTracks; i++)
		{
			TimesForReplace.push_back((int*)&ModelBytes[offset]);
			offset += (tmpTrack.InterpolationType > 1 ? 52 : 20);
		}
	}

	if (memcmp(&ModelBytes[offset], "KGSC", 4) == 0)
	{
		offset += 4;
		std::memcpy(&tmpTrack, &ModelBytes[offset], sizeof(Mdx_Track));
		offset += sizeof(Mdx_Track);
		for (int i = 0; i < tmpTrack.NrOfTracks; i++)
		{
			TimesForReplace.push_back((int*)&ModelBytes[offset]);
			offset += (tmpTrack.InterpolationType > 1 ? 40 : 16);
		}
	}


	if (memcmp(&ModelBytes[offset], "KGAO", 4) == 0)
	{
		offset += 4;
		std::memcpy(&tmpTrack, &ModelBytes[offset], sizeof(Mdx_Track));
		offset += sizeof(Mdx_Track);
		for (int i = 0; i < tmpTrack.NrOfTracks; i++)
		{
			TimesForReplace.push_back((int*)&ModelBytes[offset]);
			offset += (tmpTrack.InterpolationType > 1 ? 16 : 8);
		}
	}

	if (memcmp(&ModelBytes[offset], "KGAC", 4) == 0)
	{
		offset += 4;
		std::memcpy(&tmpTrack, &ModelBytes[offset], sizeof(Mdx_Track));
		offset += sizeof(Mdx_Track);
		for (int i = 0; i < tmpTrack.NrOfTracks; i++)
		{
			TimesForReplace.push_back((int*)&ModelBytes[offset]);
			offset += (tmpTrack.InterpolationType > 1 ? 16 : 8);
		}
	}

}


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


void ProcessMdx(std::string filename, unsigned char** OutDataPointer, size_t* OutSize, int unknown)
{
	unsigned char* ModelBytes = *OutDataPointer;
	size_t sz = *OutSize;



	for (unsigned int i = 0; i < ModelSequenceValueList.size(); i++)
	{
		ModelSequenceValueStruct mdlfix = ModelSequenceValueList[i];
		if (filename == mdlfix.FilePath)
		{
			size_t offset = 0;
			if (memcmp(&ModelBytes[offset], "MDLX", 4) == 0)
			{
				offset += 4;
				while (offset < sz)
				{
					if (memcmp(&ModelBytes[offset], "SEQS", 4) == 0)
					{
						Mdx_Sequence tmpSequence;

						offset += 4;

						size_t currenttagsize = *(size_t*)&ModelBytes[offset];
						size_t SequencesCount = currenttagsize / sizeof(Mdx_Sequence);

						size_t newoffset = offset + currenttagsize;
						offset += 4;
						while (SequencesCount > 0)
						{
							SequencesCount--;
							std::memcpy(&tmpSequence, &ModelBytes[offset], sizeof(Mdx_Sequence));

							if (mdlfix.AnimationName.length() == 0 || mdlfix.AnimationName == tmpSequence.Name)
							{
								size_t NeedPatchOffset = offset + 104 + (mdlfix.Indx * 4);
								*(float*)&ModelBytes[NeedPatchOffset] = mdlfix.Value;
							}

							offset += sizeof(Mdx_Sequence);
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


			/*	if ( IsKeyPressed( '0' ) && FileExist( ".\\Test1234.mdx" ) )
				{
					FILE *f;
					fopen_s( &f, ".\\Test1234.mdx", "wb" );
					fwrite( ModelBytes, sz, 1, f );
					fclose( f );
					MessageBoxA( 0, "Ok dump", "DUMP", 0 );
				}
	*/

			ModelSequenceValueList.erase(ModelSequenceValueList.begin() + (int)i);

		}

	}

	for (unsigned int i = 0; i < ModelSequenceReSpeedList.size(); i++)
	{
		ModelSequenceReSpeedStruct mdlfix = ModelSequenceReSpeedList[i];
		if (filename == mdlfix.FilePath)
		{

			int SequenceID = 0;
			int ReplaceSequenceID = -1;

			// First find Animation and shift others
			std::vector<Mdx_SequenceTime> Sequences;

			// Next find all objects with Node struct and shift 
			std::vector<int*> TimesForReplace;

			// Shift any others animations
			// Next need search and shift needed animation
			size_t offset = 0;
			if (memcmp(&ModelBytes[offset], "MDLX", 4) == 0)
			{
				offset += 4;
				while (offset < sz)
				{
					if (memcmp(&ModelBytes[offset], "SEQS", 4) == 0)
					{

						Mdx_Sequence tmpSequence;

						offset += 4;

						size_t currenttagsize = *(size_t*)&ModelBytes[offset];
						size_t SequencesCount = currenttagsize / sizeof(Mdx_Sequence);

						size_t newoffset = offset + currenttagsize;
						offset += 4;
						while (SequencesCount > 0)
						{
							SequencesCount--;
							std::memcpy(&tmpSequence, &ModelBytes[offset], sizeof(Mdx_Sequence));


							if (mdlfix.AnimationName == tmpSequence.Name)
							{
								ReplaceSequenceID = SequenceID;
							}


							Mdx_SequenceTime CurrentSequenceTime;
							CurrentSequenceTime.IntervalStart = (int*)&ModelBytes[offset + 80];
							CurrentSequenceTime.IntervalEnd = (int*)&ModelBytes[offset + 84];
							Sequences.push_back(CurrentSequenceTime);

							offset += sizeof(Mdx_Sequence);
							SequenceID++;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "BONE", 4) == 0)
					{
						offset += 4;
						size_t newoffset = offset + *(size_t*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						while (newoffset > offset)
						{
							std::memcpy(&tmpNode, &ModelBytes[offset], sizeof(Mdx_Node));
							ProcessNodeAnims(ModelBytes, offset + sizeof(Mdx_Node), TimesForReplace);
							offset += tmpNode.InclusiveSize + 8;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "HELP", 4) == 0)
					{
						offset += 4;
						size_t newoffset = offset + *(size_t*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						while (newoffset > offset)
						{
							std::memcpy(&tmpNode, &ModelBytes[offset], sizeof(Mdx_Node));
							ProcessNodeAnims(ModelBytes, offset + sizeof(Mdx_Node), TimesForReplace);
							offset += tmpNode.InclusiveSize;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "LITE", 4) == 0)
					{
						offset += 4;
						size_t newoffset = offset + *(size_t*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						while (newoffset > offset)
						{
							size_t size_of_this_struct = *(size_t*)&ModelBytes[offset];
							offset += 4;
							std::memcpy(&tmpNode, &ModelBytes[offset], sizeof(Mdx_Node));
							ProcessNodeAnims(ModelBytes, offset + sizeof(Mdx_Node), TimesForReplace);
							offset += size_of_this_struct - 4;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "ATCH", 4) == 0)
					{
						offset += 4;
						size_t newoffset = offset + *(size_t*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						while (newoffset > offset)
						{
							size_t size_of_this_struct = *(size_t*)&ModelBytes[offset];
							offset += 4;
							std::memcpy(&tmpNode, &ModelBytes[offset], sizeof(Mdx_Node));
							ProcessNodeAnims(ModelBytes, offset + sizeof(Mdx_Node), TimesForReplace);
							offset += size_of_this_struct - 4;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "PREM", 4) == 0)
					{
						offset += 4;
						size_t newoffset = offset + *(size_t*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						while (newoffset > offset)
						{
							size_t size_of_this_struct = *(size_t*)&ModelBytes[offset];
							offset += 4;
							std::memcpy(&tmpNode, &ModelBytes[offset], sizeof(Mdx_Node));
							ProcessNodeAnims(ModelBytes, offset + sizeof(Mdx_Node), TimesForReplace);
							offset += size_of_this_struct - 4;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "PRE2", 4) == 0)
					{
						offset += 4;
						size_t newoffset = offset + *(size_t*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						while (newoffset > offset)
						{
							size_t size_of_this_struct = *(size_t*)&ModelBytes[offset];
							offset += 4;
							std::memcpy(&tmpNode, &ModelBytes[offset], sizeof(Mdx_Node));
							ProcessNodeAnims(ModelBytes, offset + sizeof(Mdx_Node), TimesForReplace);
							offset += size_of_this_struct - 4;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "GEOA", 4) == 0)
					{
						Mdx_GeosetAnimation tmpGeosetAnimation;
						offset += 4;
						size_t newoffset = offset + *(size_t*)&ModelBytes[offset];
						offset += 4;
						while (newoffset > offset)
						{
							std::memcpy(&tmpGeosetAnimation, &ModelBytes[offset], sizeof(Mdx_GeosetAnimation));
							ProcessNodeAnims(ModelBytes, offset + sizeof(Mdx_GeosetAnimation), TimesForReplace);

							offset += tmpGeosetAnimation.InclusiveSize;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "RIBB", 4) == 0)
					{
						offset += 4;
						size_t newoffset = offset + *(size_t*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						while (newoffset > offset)
						{
							size_t size_of_this_struct = *(size_t*)&ModelBytes[offset];
							offset += 4;
							std::memcpy(&tmpNode, &ModelBytes[offset], sizeof(Mdx_Node));
							ProcessNodeAnims(ModelBytes, offset + sizeof(Mdx_Node), TimesForReplace);
							offset += size_of_this_struct - 4;
						}
						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "EVTS", 4) == 0)
					{
						offset += 4;
						size_t newoffset = offset + *(size_t*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						Mdx_Tracks tmpTracks;
						while (newoffset > offset)
						{
							std::memcpy(&tmpNode, &ModelBytes[offset], sizeof(Mdx_Node));
							ProcessNodeAnims(ModelBytes, offset + sizeof(Mdx_Node), TimesForReplace);
							offset += tmpNode.InclusiveSize;
							if (memcmp(&ModelBytes[offset], "KEVT", 4) == 0)
							{
								offset += 4;
								std::memcpy(&tmpTracks, &ModelBytes[offset], sizeof(Mdx_Tracks));
								offset += sizeof(Mdx_Tracks);
								for (int n = 0; n < tmpTracks.NrOfTracks; n++)
								{
									TimesForReplace.push_back((int*)&ModelBytes[offset]);
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
						size_t newoffset = offset + *(size_t*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						while (newoffset > offset)
						{
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
					else
					{
						offset += 4;
						offset += *(int*)&ModelBytes[offset];
					}

					offset += 4;
				}
			}

			if (ReplaceSequenceID != -1)
			{

				int SeqEndTime = *Sequences[(unsigned int)ReplaceSequenceID].IntervalEnd;
				int SeqStartTime = *Sequences[(unsigned int)ReplaceSequenceID].IntervalStart;
				int NewEndTime = SeqStartTime + (int)((SeqEndTime - SeqStartTime) / mdlfix.SpeedUp);
				int AddTime = NewEndTime - SeqEndTime;

				for (unsigned int n = 0; n < Sequences.size(); n++)
				{
					if (*Sequences[n].IntervalStart >= SeqEndTime)
					{
						*Sequences[n].IntervalStart += AddTime;
						*Sequences[n].IntervalEnd += AddTime;
					}
				}

				*Sequences[(unsigned int)ReplaceSequenceID].IntervalEnd = NewEndTime;

				for (int* dwTime : TimesForReplace)
				{
					if (*dwTime >= SeqEndTime)
					{
						*dwTime += AddTime;
					}
					else if (*dwTime <= SeqEndTime && *dwTime >= SeqStartTime)
					{
						*dwTime = (int)SeqStartTime + (int)((float)(*dwTime - SeqStartTime) / mdlfix.SpeedUp);
					}
				}


				/*if ( IsKeyPressed( '0' ) && FileExist( ".\\Test1234.mdx" ) )
				{
					FILE *f;
					fopen_s( &f, ".\\Test1234.mdx", "wb" );
					fwrite( ModelBytes, sz, 1, f );
					fclose( f );
					MessageBoxA( 0, "Ok dump", "DUMP", 0 );
				}*/

			}

			if (!TimesForReplace.empty())
				TimesForReplace.clear();
			if (!Sequences.empty())
				Sequences.clear();

			ModelSequenceReSpeedList.erase(ModelSequenceReSpeedList.begin() + (int)i);

		}

	}


	for (unsigned int i = 0; i < ModelRemoveTagList.size(); i++)
	{
		ModelRemoveTagStruct mdlfix = ModelRemoveTagList[i];
		if (filename == mdlfix.FilePath)
		{
			int TagFound = false;
			size_t TagStartOffset = 0;
			size_t TagSize = 0;
			size_t offset = 0;
			if (memcmp(&ModelBytes[offset], "MDLX", 4) == 0)
			{
				offset += 4;
				while (offset < sz)
				{
					if (memcmp(&ModelBytes[offset], mdlfix.TagName.c_str(), 4) == 0)
					{

						TagFound = true;
						TagStartOffset = offset;
						offset += 4;
						offset += *(int*)&ModelBytes[offset];
						TagSize = offset - TagStartOffset;

					}
					else
					{
						offset += 4;
						offset += *(int*)&ModelBytes[offset];
					}

					offset += 4;
				}

			}

			if (TagFound)
			{
				std::memcpy(&ModelBytes[TagStartOffset], &ModelBytes[TagStartOffset + TagSize + 4], sz - (TagStartOffset + TagSize));
				memset(&ModelBytes[sz - TagSize - 4], 0xFF, TagSize);

				sz = sz - TagSize - 4;
				*OutSize = sz;
			}


			ModelRemoveTagList.erase(ModelRemoveTagList.begin() + (int)i);

		}
	}


	for (unsigned int i = 0; i < ModelScaleList.size(); i++)
	{
		ModelScaleStruct mdlfix = ModelScaleList[i];
		if (filename == mdlfix.FilePath)
		{
			if (!FullPatchData.empty())
				FullPatchData.clear();

			char TagName[5];
			memset(TagName, 0, 5);
			size_t offset = 0;

			unsigned long MaxObjectId = 0;

			std::vector<unsigned long*> parents;

			unsigned long OffsetToInsertPivotPoint = 0;

			int FoundGLBS = false;
			const char* strGLBS = "GLBS";

			if (memcmp(&ModelBytes[offset], "MDLX", 4) == 0)
			{
				offset += 4;
				while (offset < sz)
				{
					std::memcpy(TagName, &ModelBytes[offset], 4);
					if (memcmp(&ModelBytes[offset], strGLBS, 4) == 0)
					{
						offset += 4;
						size_t newoffset = offset + *(size_t*)&ModelBytes[offset];
						FoundGLBS = true;

						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "PIVT", 4) == 0)
					{
						offset += 4;
						size_t newoffset = offset + *(size_t*)&ModelBytes[offset];

						*(size_t*)&ModelBytes[offset] = 12 + *(size_t*)&ModelBytes[offset];

						OffsetToInsertPivotPoint = newoffset + 4;

						offset = newoffset;
					}
					else if (memcmp(&ModelBytes[offset], "BONE", 4) == 0)
					{
						offset += 4;
						size_t newoffset = offset + *(size_t*)&ModelBytes[offset];
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
						size_t newoffset = offset + *(size_t*)&ModelBytes[offset];
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
						size_t newoffset = offset + *(size_t*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						while (newoffset > offset)
						{
							size_t size_of_this_struct = *(size_t*)&ModelBytes[offset];
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
						size_t newoffset = offset + *(size_t*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						//Mdx_Tracks tmpTracks;
						while (newoffset > offset)
						{
							size_t size_of_this_struct = *(size_t*)&ModelBytes[offset];
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
						size_t newoffset = offset + *(size_t*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						while (newoffset > offset)
						{
							size_t size_of_this_struct = *(size_t*)&ModelBytes[offset];
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
						size_t newoffset = offset + *(size_t*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						while (newoffset > offset)
						{
							size_t size_of_this_struct = *(size_t*)&ModelBytes[offset];
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
						size_t newoffset = offset + *(size_t*)&ModelBytes[offset];
						offset += 4;
						Mdx_Node tmpNode;
						while (newoffset > offset)
						{
							size_t size_of_this_struct = *(size_t*)&ModelBytes[offset];
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
						size_t newoffset = offset + *(size_t*)&ModelBytes[offset];
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
						size_t newoffset = offset + *(size_t*)&ModelBytes[offset];
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

			FullPatchData.insert(FullPatchData.end(), (unsigned char*)(ModelBytes), (unsigned char*)(ModelBytes + sz));

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


			//if ( IsKeyPressed( '0' ) && FileExist( ".\\Test1234.mdx" ) )
			//{
			//	FILE *f;
			//	fopen_s( &f, ".\\Test1234.mdx", "wb" );
			//	fwrite( &FullPatchData[ 0 ], FullPatchData.size( ), 1, f );
			//	fclose( f );
			//	MessageBoxA( 0, "Ok dump", "DUMP", 0 );
			//}


			ICONMDLCACHE* tmpih = Storm::MemAllocStruct<ICONMDLCACHE>();
			StormBuffer ResultBuffer;
			ResultBuffer.buf = (unsigned char*)Storm::MemAlloc(FullPatchData.size());
			ResultBuffer.length = FullPatchData.size();
			std::memcpy(&ResultBuffer.buf[0], &FullPatchData[0], FullPatchData.size());

			tmpih->buf = ResultBuffer.buf;
			tmpih->size = ResultBuffer.length;
			tmpih->hashlen = filename.length();
			tmpih->_hash = GetBufHash(filename.c_str(), tmpih->hashlen);
			ICONMDLCACHELIST.push_back(*tmpih);

			if (NeedReleaseUnusedMemory)
				Storm::MemFree((void*)*OutDataPointer);

			*OutDataPointer = tmpih->buf;
			*OutSize = tmpih->size;

			ModelBytes = (unsigned char*)tmpih->buf;
			sz = tmpih->size;

			Storm::MemFree(tmpih);
			ModelScaleList.erase(ModelScaleList.begin() + (int)i);

		}
	}


	if (!FullPatchData.empty())
		FullPatchData.clear();
	for (unsigned int i = 0; i < ModelPatchList.size(); i++)
	{
		ModelPatchStruct mdlfix = ModelPatchList[i];
		if (filename == mdlfix.FilePath)
		{
			unsigned char * PatchFileData;
			size_t PatchFileSize;

			if (GameGetFile_ptr(mdlfix.patchPath.c_str(), &PatchFileData, &PatchFileSize, unknown))
			{
				FullPatchData.insert(FullPatchData.end(), (char*)(PatchFileData), (char*)(PatchFileData + PatchFileSize));
			}


			ModelPatchList.erase(ModelPatchList.begin() + (int)i);

		}
	}

	if (!FullPatchData.empty())
	{

		ICONMDLCACHE* tmpih = Storm::MemAllocStruct< ICONMDLCACHE>();
		int FoundOldHelper = GetFromIconMdlCache(filename, tmpih);


		if (FoundOldHelper)
		{
			StormBuffer ResultBuffer;
			ResultBuffer.buf = (unsigned char*)Storm::MemAlloc(tmpih->size + FullPatchData.size());

			ResultBuffer.length = tmpih->size + FullPatchData.size();

			std::memcpy(&ResultBuffer.buf[0], tmpih->buf, sz);

			std::memcpy(&ResultBuffer.buf[sz], &FullPatchData[0], FullPatchData.size());

			Storm::MemFree(tmpih->buf);
			tmpih->buf = ResultBuffer.buf;
			tmpih->size = ResultBuffer.length;
			*OutDataPointer = tmpih->buf;
			*OutSize = tmpih->size;
		}
		else
		{
			StormBuffer ResultBuffer;
			ResultBuffer.buf = (unsigned char*)Storm::MemAlloc(sz + FullPatchData.size());
			ResultBuffer.length = sz + FullPatchData.size();
			std::memcpy(&ResultBuffer.buf[0], ModelBytes, sz);
			std::memcpy(&ResultBuffer.buf[sz], &FullPatchData[0], FullPatchData.size());

			tmpih->buf = ResultBuffer.buf;
			tmpih->size = ResultBuffer.length;

			tmpih->hashlen = filename.length();
			tmpih->_hash = GetBufHash(filename.c_str(), tmpih->hashlen);
			ICONMDLCACHELIST.push_back(*tmpih);

			if (NeedReleaseUnusedMemory)
				Storm::MemFree((void*)*OutDataPointer);


			*OutDataPointer = tmpih->buf;
			*OutSize = tmpih->size;

			ModelBytes = (unsigned char*)tmpih->buf;
			sz = tmpih->size;

		}

		if (IsKeyPressed('0') && FileExist(".\\Test1234.mdx"))
		{
			FILE* f;
			fopen_s(&f, ".\\Test1234.mdx", "wb");
			fwrite(ModelBytes, sz, 1, f);
			fclose(f);
		}

		Storm::MemFree(tmpih);
		FullPatchData.clear();
	}


	for (unsigned int i = 0; i < ModelCollisionFixList.size(); i++)
	{
		ModelCollisionFixStruct mdlfix = ModelCollisionFixList[i];
		if (filename == mdlfix.FilePath)
		{
			size_t offset = 0;
			if (memcmp(&ModelBytes[offset], "MDLX", 4) == 0)
			{
				offset += 4;
				while (offset < sz)
				{
					if (memcmp(&ModelBytes[offset], "CLID", 4) == 0)
					{
						offset += 4;
						size_t newoffset = offset + *(int*)&ModelBytes[offset];
						offset += 4;
						offset += *(int*)&ModelBytes[offset];
						int shapetype = *(int*)&ModelBytes[offset];

						if (shapetype == 2)
						{
							offset += 4;
							*(float*)&ModelBytes[offset] = mdlfix.X;
							offset += 4;
							*(float*)&ModelBytes[offset] = mdlfix.Y;
							offset += 4;
							*(float*)&ModelBytes[offset] = mdlfix.Z;
							offset += 4;
							*(float*)&ModelBytes[offset] = mdlfix.Radius;
						}
						offset = newoffset + 4;
					}
					else
					{
						offset += 4;
						offset += *(int*)&ModelBytes[offset];
					}
					offset += 4;
				}
			}

			ModelCollisionFixList.erase(ModelCollisionFixList.begin() + (int)i);

		}

	}


	for (unsigned int i = 0; i < ModelTextureFixList.size(); i++)
	{
		ModelTextureFixStruct mdlfix = ModelTextureFixList[i];
		int TextureID = 0;
		if (filename == mdlfix.FilePath)
		{
			size_t offset = 0;
			if (memcmp(&ModelBytes[offset], "MDLX", 4) == 0)
			{
				offset += 4;
				while (offset < sz)
				{
					if (memcmp(&ModelBytes[offset], "TEXS", 4) == 0)
					{
						Mdx_Texture tmpTexture;

						//char TextNameBuf[ 0x100 ];
						offset += 4;
						int TagSize = *(int*)&ModelBytes[offset];
						size_t newoffset = offset + TagSize;
						int TexturesCount = TagSize / (int)sizeof(Mdx_Texture);
						offset += 4;


						while (TexturesCount > 0)
						{
							TexturesCount--;
							std::memcpy(&tmpTexture, &ModelBytes[offset], sizeof(Mdx_Texture));
							TextureID++;

							if (mdlfix.TextureID == TextureID)
							{
								if (mdlfix.NewTexturePath.length() > 3)
								{
									tmpTexture.ReplaceableId = 0;
									sprintf_s(tmpTexture.FileName, 260, "%s", mdlfix.NewTexturePath.c_str());
								}
								else
								{
									tmpTexture.ReplaceableId = atoi(mdlfix.NewTexturePath.c_str());
									memset(tmpTexture.FileName, 0, 260);
								}
								std::memcpy(&ModelBytes[offset], &tmpTexture, sizeof(Mdx_Texture));
							}
							offset += sizeof(Mdx_Texture);
						}
						offset = newoffset + 4;
					}
					else
					{
						offset += 4;
						offset += *(int*)&ModelBytes[offset];
					}
					offset += 4;
				}
			}



			ModelTextureFixList.erase(ModelTextureFixList.begin() + (int)i);

		}

	}


}


int __stdcall RedirectFile(const char* RealFilePath, const char* NewFilePath)
{
	FileRedirectStruct tmpModelFix;
	tmpModelFix.NewFilePath = NewFilePath;
	tmpModelFix.RealFilePath = RealFilePath;
	FileRedirectList.push_back(tmpModelFix);
	return 0;
}

void PrintLog(const char* str)
{
	FILE* f;

	fopen_s(&f, ".\\text.txt", "a+");
	if (f != NULL)
	{
		fprintf_s(f, "%s\n", str);
		fclose(f);
	}
}


int ProcessFile(const char * filename, unsigned char ** OutDataPointer, size_t* OutSize, int unknown, int IsFileExistOld)
{
	//PrintLog( filename.c_str( ) );
	int IsFileExist = IsFileExistOld;

	if (!OutDataPointer || !OutSize || !filename)
	{
		return IsFileExist;
	}

	ICONMDLCACHE tmpih;
	int FoundOldHelper = GetFromIconMdlCache(filename, &tmpih);
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
			if (_stricmp(filename,s.filename) == 0)
			{
				*OutDataPointer = s.ingamebuffer.buf;
				*OutSize = s.ingamebuffer.length;
				return true;
			}
		}
	}



	std::string FileExtension = ToLower(fs::path(filename).extension().string());


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
			ProcessMdx(filename, OutDataPointer, OutSize, unknown);
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
						ProcessMdx(filename, OutDataPointer, OutSize, unknown);
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
//					memcpy( buffer, s.ingamebuffer.buf, size );
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

	if (TerminateStarted)
	{
		return NULL;
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
	ApplyIconFrameFilter(iconpath, 0, 0);


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

std::string GetFileContent(std::string filename)
{
	std::ifstream t(filename);
	std::string str;

	t.seekg(0, std::ios::end);

	if (t.tellg() > NULL)
	{
		str.reserve((size_t)t.tellg());
		t.seekg(0, std::ios::beg);

		str.assign((std::istreambuf_iterator<char>(t)),
			std::istreambuf_iterator<char>());
	}

	return str;
}


std::vector<std::string> get_file_list(const fs::path& path, bool dotolower)
{
	std::vector<std::string> m_file_list;
	if (!path.empty())
	{
		fs::path apk_path(path);
		fs::recursive_directory_iterator end;

		for (fs::recursive_directory_iterator i(apk_path); i != end; ++i)
		{
			const fs::path cp = (*i);

			m_file_list.push_back(dotolower ? ToLower(cp.string()) : cp.string());
		}
	}
	return m_file_list;
}



int __stdcall IsLocalFilesEnabled(int)
{
	if (GameDll)
		return *(int*)GameDll + 0xAAE2AC;
	return false;
}

typedef void(__fastcall* EnableLocalFiles_p)(int enable);
EnableLocalFiles_p EnableLocalFiles_org = NULL;

void __stdcall EnableLocalFiles(int enable)
{
	EnableLocalFiles_org = (EnableLocalFiles_p)(GameDll + 0x0010B0);
	EnableLocalFiles_org(enable);
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