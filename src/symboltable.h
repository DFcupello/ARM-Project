
#define MAX_LINE_LENGTH 511

typedef struct Label {
  char label[MAX_LINE_LENGTH];
  uint32_t address;
} SymbolItem;

void addItem(int labelCount, char *label, int size, uint32_t address, SymbolItem **symboltable);

uint32_t getAddress(char *label, SymbolItem *symbolTable);

void freeSymbolTable(SymbolItem **symboltable);

void printTable(int index, SymbolItem **symbolTable);
