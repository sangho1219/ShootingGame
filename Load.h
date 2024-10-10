bool strcmp_A(char* arg1, char* arg2);
int TenSquare(int a);

int strChangeInt(char* str);

int strlen_null(const char* src);
int strlen_space(const char* src);


void spaceChangeNull(char** Buffer);
int spaceAmount(const char* Buffer);


int newLineAmount(const char* Buffer);
void newLineChangeNull(char** Buffer);

bool Init_nullList(char* Buffer_arg, int size, char*** List_arg);
void Free_nullList(char** List_arg);


bool strcpy_malloc(const char* src, char** dest);
void strcpy_free(char* dest);


bool Load_File(char** Buffer, const char* name);
void Free_File(char* Buffer);

void InfoList(char** InfoList, int typeAmount, char**** List, char*** ListData);


