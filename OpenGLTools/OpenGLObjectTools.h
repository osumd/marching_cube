#ifndef OPENGL_OBJECT_TOOLS
#define OPENGL_OBJECT_TOOLS 

#include <stdio.h>
#include <parseVol.h>



#pragma region OpenGLBuffer

std::string ParseShaderWithIncludes(const char* path, const char* rootpath);

char* ParseShader(const char* path)
{
	FILE* ParseFile;
	fopen_s(&ParseFile, path, "rb");
	fpos_t extractionByteTotal;
	if (ParseFile != NULL)
	{
		fseek(ParseFile, 0, SEEK_END);
		fgetpos(ParseFile, &extractionByteTotal);
		fseek(ParseFile, 0, SEEK_SET);
		char* ParseBuffer = (char*)malloc(static_cast<size_t>(extractionByteTotal + 1));
		if (ParseBuffer != nullptr)
		{
			fread(ParseBuffer, extractionByteTotal, 1, ParseFile);
			ParseBuffer[extractionByteTotal] = '\0';
		}
		fclose(ParseFile);
		return ParseBuffer;
	}
	return nullptr;
}



bool isInclude(FILE* f)
{
	int advancedForward = 1;
	int c = fgetc(f);

	const char* itt = "nclude";
	if(c == 'i')
	{

		int ncludeIndex = 0;
		c = fgetc(f);
		advancedForward++;

		while(c == itt[ncludeIndex] && ncludeIndex < 5)
		{
			ncludeIndex++;
			c = fgetc(f);
			advancedForward++;
		}

		if(ncludeIndex == 5)
		{
			return true;
		}
	}

	fseek(f, -advancedForward, SEEK_CUR);
	return false;

}

bool replaceWithFile(FILE* f, const char* rootpath, std::string& sourceFile)
{
	int c = fgetc(f);
	std::string fileName(rootpath);

	int advancement = 1;

	if(c == 32)
	{
		c = fgetc(f);
		advancement++;

		while(c == 32 && c != EOF)
		{
			c = fgetc(f);
			advancement++;
		}
	}

	if(c != 34)
	{
		fseek(f, -advancement, SEEK_CUR);
		return false;
	}else
	{
		c = fgetc(f);

		while(c != 34 && c != EOF)
		{
			fileName += c;
			c = fgetc(f);
		}
		

		
		std::string fileSource = ParseShaderWithIncludes(fileName.c_str(), rootpath);

		sourceFile += fileSource;
		
		fseek(f, 1, SEEK_CUR);
		return true;
	}

	return false;

}

std::string ParseShaderWithIncludes(const char* path, const char* rootpath)
{
	FILE* ParseFile;
	fopen_s(&ParseFile, path, "rb");
	fpos_t extractionByteTotal;

	std::string sourceCode;

	if (ParseFile != NULL)
	{

		int FileCharacter = fgetc(ParseFile);
		while(FileCharacter != EOF)
		{
			
			if(FileCharacter == '#')
			{
				bool isIncluded = isInclude(ParseFile);
				if(isIncluded)
				{
					
					replaceWithFile(ParseFile, rootpath, sourceCode);
					FileCharacter = fgetc(ParseFile);
				}


			}
			sourceCode += FileCharacter;

			FileCharacter = fgetc(ParseFile);

		}

		
		return sourceCode.data();
	}
	return "";
}

#pragma endregion


#endif