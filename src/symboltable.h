typedef struct Label {
  char *label;
  uint32_t address;
} SymbolItem;

SymbolItem* buildSymbolTable();

void addItem(int labelCount, char *label, uint32_t address, SymbolItem **symboltable);

void getItem(char *label, SymbolItem **symbolTable);

void freeSymbolTable(SymbolItem **symboltable);

void printTable(int index, SymbolItem **symbolTable);
