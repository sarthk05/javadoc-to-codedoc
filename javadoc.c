#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* convert_javadoc_to_codedoc(char* javadoc) {
    // Find the start of the comment block
    char* start = strstr(javadoc, "/**");
    if (start == NULL) {
        return javadoc;
    }

    // Find the end of the comment block
    char* end = strstr(start, "*/");
    if (end == NULL) {
        return javadoc;
    }

    // Extract the comment block
    size_t block_len = end - start + 2;
    char* block = (char*)malloc((block_len + 1) * sizeof(char));
    strncpy(block, start, block_len);
    block[block_len] = '\0';

    // Find the function name
    char* func_name_start = strchr(javadoc, ' ') + 1;
    char* func_name_end = strchr(func_name_start, '(');
    if (func_name_start == NULL || func_name_end == NULL) {
        return javadoc;
    }
    size_t func_name_len = func_name_end - func_name_start;
    char* func_name = (char*)malloc((func_name_len + 1) * sizeof(char));
    strncpy(func_name, func_name_start, func_name_len);
    func_name[func_name_len] = '\0';

    // Find the @param and @return lines
    char* param_start = strstr(block, "@param");
    char* return_start = strstr(block, "@return");
    if (param_start == NULL || return_start == NULL) {
        return javadoc;
    }

    // Generate the codedoc string
    size_t codedoc_len = block_len + func_name_len + strlen("() - ") + strlen(return_start) + strlen(param_start);
    char* codedoc = (char*)malloc((codedoc_len + 1) * sizeof(char));
    sprintf(codedoc, "/**\n *  '%s{}' - ", func_name);
    strncat(codedoc, block + 3, block_len - 6);
    strcat(codedoc, "\n");
    strcat(codedoc, return_start);
    strcat(codedoc, "\n");
    strcat(codedoc, func_name);
    strcat(codedoc, "(");
    char* param_name = strtok(param_start, "\n");
    while (param_name != NULL) {
        strcat(codedoc, param_name + strlen(" * @param "));
        strcat(codedoc, ",\n     ");
        param_name = strtok(NULL, "\n");
    }
    codedoc[strlen(codedoc) - 2] = ')';
    strcat(codedoc, " //");
    strcat(codedoc, return_start + strlen(" * @return"));

    // Free memory and return codedoc string
    free(block);
    free(func_name);
    return codedoc;
}
