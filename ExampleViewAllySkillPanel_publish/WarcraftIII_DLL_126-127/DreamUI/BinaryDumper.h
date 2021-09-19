#include "stdafx.h"
#ifndef BINARY_DUMPER_H_
#define BINARY_DUMPER_H_

typedef uint8_t byte;

class BinaryDumper {
public:
	BinaryDumper(const char* outFilename) {
		static bool ConsoleInited = false;
		if (outFilename) {
			if (fopen_s(&this->fp_, outFilename, "w")) {
				this->fp_ = NULL;
#ifndef _VMP
				;//OutputDebugString("BinaryDumper: Unable to open file.");
#endif
			}
		} else {
			if (!ConsoleInited) {
				if (!AllocConsole())
					MessageBox(NULL, "Alloc Console Failed.", "Error", 0);
				else {
					ConsoleInited = true;
					freopen_s(&this->fp_, "CONIN$", "r+t", stdin);	// 重定向STDIN
					freopen_s(&this->fp_, "CONOUT$", "w+t", stdout);	// 重定向STDOUT
				}
			} else {
				freopen_s(&this->fp_, "CONIN$", "r+t", stdin);		// 重定向STDIN
				freopen_s(&this->fp_, "CONOUT$", "w+t", stdout);	// 重定向STDOUT
			}
		}
	}

	void dump(void* offset, int length, const char* title = NULL) {//禁止public release输出
#ifndef _VMP
		if (!this->fp_)
			return;

		int count = 0;
		char buffer[100];

		if (title) {
			fprintf(this->fp_, "[%s]\n", title);
		}

		fprintf(this->fp_, "===========================================================================\n");
		fprintf(this->fp_, "Address: 0x%X     Length: %d\n", (unsigned int)offset, length);
		fprintf(this->fp_, "---------------------------------------------------------------------------\n");
		fprintf(this->fp_, "00 01 02 03 | 04 05 06 07 | 08 09 0A 0B | 0C 0D 0E 0F || 0123456789ABCDEF\n");
		fprintf(this->fp_, "---------------------------------------------------------------------------\n");
		for(int i = 0; i < length; i += 0x10) {
			if (this->FormatDataLine(buffer, 100, static_cast<byte*>(offset) + i, i + 0x10 < length ? 0x10 : length - i))//;
				fprintf(this->fp_, "%s\n", buffer);
		}

		fprintf(this->fp_, "===========================================================================\n\n\n");

		fflush(this->fp_);

		this->count_++;
#endif
	}

	~BinaryDumper() {
		if (this->fp_)
			fclose(this->fp_);
	}
private:
	FILE* fp_;
	uint32_t count_;

	bool FormatDataLine(char* dst, int dst_size, byte* data, int size) {
		int offset = 0;
		byte value;
		char hex_map[] = "0123456789ABCDEF";

		if (size > 0x10)
			return false;

		memset(dst, ' ', dst_size);

		//hex
		for (int i = 0; i < size; i++) {
			value = data[i];

			//HEX DUMP
			dst[offset++] = hex_map[value / 0x10];
			dst[offset++] = hex_map[value % 0x10];

			//每个数一个空格间隔，除了最后一个
			if ((i + 1) != 0x10)
				dst[offset++] = ' ';

			//4个一组
			if ((i + 1) % 0x4 == 0 && i) {
				dst[offset++] = ' ';
				dst[offset++] = ' ';
			}
		}

		//分隔
		dst[54] = '|';
		dst[55] = '|';
		//空一格
		offset = 57;

		//chr
		for (int i = 0; i < size; i++) {
			value = data[i];
			dst[offset++] = (value >= 0x20 && value <= 0x7E) ? value : ' ';
		}

		dst[offset] = 0;

		return true;
	}

	DISALLOW_COPY_AND_ASSIGN(BinaryDumper);
};

#endif //BINARY_DUMPER_H_