#include<windows.h>
#include<stdio.h>
#include <inttypes.h>
#include <malloc.h>

int main()
{
    FILE* txt_output = fopen("output/meta_data.txt", "w");
    FILE* exe_file = fopen("hallo.exe", "rb");
    struct _IMAGE_DOS_HEADER dos_Header = {0};
    struct _IMAGE_NT_HEADERS pe_header = {0};

    fread(&dos_Header, sizeof (struct _IMAGE_DOS_HEADER), 1, exe_file);

    if(dos_Header.e_magic != 23117){
        char* message = "It's not an exe file";
        fprintf(txt_output, "%s", message);

        fclose(txt_output);
        return 0;
    }
    fseek(exe_file, dos_Header.e_lfanew, SEEK_SET);
    fread(&pe_header, sizeof (struct _IMAGE_NT_HEADERS), 1, exe_file);

    if(pe_header.Signature != 17744){
        char* message = "Incorrect PE header";
        fprintf(txt_output, "%s", message);
        fclose(txt_output);
        return 0;
    }


    fprintf(txt_output, "Sections: %lu\n", pe_header.FileHeader.NumberOfSections);
    fprintf(txt_output, "Entry: %lu", pe_header.OptionalHeader.AddressOfEntryPoint);
    fclose(txt_output);


    IMAGE_SECTION_HEADER *section_headers = malloc(sizeof (struct _IMAGE_SECTION_HEADER) * pe_header.FileHeader.NumberOfSections);
    fread(section_headers,
          sizeof (IMAGE_SECTION_HEADER),
                  pe_header.FileHeader.NumberOfSections, exe_file);

    for(int i = 0; i < pe_header.FileHeader.NumberOfSections; ++i){
        char* file_name = malloc(sizeof(char) * 12);
        sprintf(file_name, "output/%s", (char*)section_headers[i].Name);
        FILE* bin = fopen(file_name, "wb");
        void *tempo = malloc(section_headers[i].SizeOfRawData);

        fseek(exe_file, section_headers[i].PointerToRawData, SEEK_SET);
        fread(tempo, section_headers[i].SizeOfRawData, 1, exe_file);
        fwrite(tempo, section_headers[i].SizeOfRawData, 1, bin);
        fclose(bin);
        free(tempo);
    }

    free(section_headers);
    fclose(exe_file);
    fclose(txt_output);
    return 0;
}