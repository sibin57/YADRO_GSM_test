/*Задан CSV-файл с заголовком, в котором перечислены названия столбцов.
 * Строки нумеруются целыми положительными числами, необязательно в порядке возрастания.
 * В ячейках CSV-файла могут хранится или целые числа или выражения вида 
 * = ARG1 OP ARG2 
 * где ARG1 и ARG2 – целые числа или адреса ячеек в формате Имя_колонки Номер_строки,
 * а OP – арифметическая операция из списка: +, -, *, /.
 *
 * требуется написать программу, которая читает произвольную CSV-форму из файла
 * (количество строк и столбцов может быть любым), вычисляет значения ячеек,
 * если это необходимо, и выводит получившуюся табличку в виде CSV-представления в консоль.
 */

//Реализовать чтение из файла и хранение информации в памяти
//Производить рассчёты уже после полного чтения данных

//Варианты ошибок:
//циклические ссылки (две ячейки ссылаются друг на друга)
//в ячейке содержится ссылка на несуществующий адрес
//в ячейке написано некорректное выражение

#define MAX_SIDE_SIZE 64  //размер стороны таблицы
#define MAX_WORD_SIZE 32  //длинна слова в ячейке

#include <stdio.h>


//копирование s в d до \0
void strcopy(char* d, char* s){
    while(*d++ = *s++) //'\0' считается как 0
        ;
}

//решение выражения A op B
int solve_exp(int a, int b, char op){
    int result = 0;
    switch(op){
        case '+':
            result = a+b;
            break;
        case '-':
            result = a-b;
            break;
        case '*':
            result = a*b;
            break;
        case '/':
            result = a/b;
            break;
        default:
            printf("illegal operation \"%c\"", op);
            break;
    }
    return result;
}


//расшифровка адреса
int parse_addr(char* addr){
    char row_name[MAX_WORD_SIZE/2], column_name[MAX_WORD_SIZE/2];
    char *prow_name = row_name;
    char *pcolumn_name = column_name;
    while(*addr<'0' || *addr>'9'){ //не число
        *prow_name = *addr;
        prow_name++;
        addr++;
        
    }
    *prow_name='\0';

    printf("got row %s\n", row_name);

    while (*pcolumn_name++ = *addr++)
        ;
    printf("got column %s\n", column_name);

    //TODO организовать возвращение полезной информации
}

//расшифровка выражения "=AopB" с поиском элементов
//более крупные выражения не поддерживаются (т.е., выражение "=AopBopC" вызовет ошибку)
int parse_exp(char* exp_str){
    char a[MAX_WORD_SIZE/2-1], b[MAX_WORD_SIZE/2-1];
    char op = ' ';
    char word[MAX_WORD_SIZE/2];
    char *pword = word;

    

    exp_str++; //пропускаем первый знак (=)
    while(*exp_str){
        if (*exp_str != '+' && *exp_str != '-' && *exp_str != '/' && *exp_str != '*'){
            *pword = *exp_str;
            pword++;
        }
        else{
            if (op == ' ') {
                op=*exp_str;
                *pword='\0';
                strcopy(a, word);
                pword = &word[0];
            }
            else {
                printf("error when deciphering expression: multiple operations in one cell");
                return 1;
            }
        }
        exp_str++;
    }

    *pword='\0';
    strcopy(b, word);

    printf("in expr deciphered a=%s op =%c b=%s\n", a, op, b);
    
    parse_addr(a);
    parse_addr(b);
    
    //TODO организовать возвращение полезной информации
}

int calc_cell(int row, int column, char table[MAX_SIDE_SIZE][MAX_SIDE_SIZE][MAX_WORD_SIZE]){
//TODO
}

int main(int argc, char *argv[]){
    
    if (argc == 1 ){
        printf("enter filename as an argument\n");
        return 0;
    }

    FILE *fp;
    printf("opening file with filename %s\n", argv[1]);

    fp = fopen(argv[1],"r");
    
    if (fp!=NULL){

        //чтение содержимого
        char table[MAX_SIDE_SIZE][MAX_SIDE_SIZE][MAX_WORD_SIZE];
        char word [MAX_WORD_SIZE] = "";//возможно не нужно
        char c;
        
        unsigned short row = 0, column = 0, letter = 0;
        unsigned short max_column = 0;
        
        //TODO обработка неполных строк
        //TODO проверка на максимальный размер стороны
        //TODO проверка на максимальный размер слова
        while ((c = getc(fp)) != EOF){
            if(c!=',' && c!='\n'){
                word[letter] = c;
                letter++;
            }
            else if(c==','){
                word[letter] = '\0';
                
                strcopy(table [row][column], word);
                
                printf("r:%d c:%d %s  ", row, column, table[row][column]);

                column++;
                letter = 0;
            }
            else if(c=='\n'){
                word[letter] = '\0';
                
                strcopy(table [row][column], word); 
                printf("r:%d c:%d %s\n", row, column, table[row][column]);
                 
                if(column > max_column)
                    max_column = column;
                row++;
                column = 0;
                letter = 0;
            }
        }

        //вывод прочитанного
        row--;
        printf("\ntotal rows: %i, total columns: %i\n", (row+1), (max_column+1));
        unsigned short i_row = 0, i_column = 0;

        while (i_row != row+1 && i_column != max_column+1){
            printf("r:%d c:%d %s", i_row, i_column, table[i_row][i_column]);

            i_column++;

            if(i_column != max_column+1) 
                printf(", ");

            else{
                i_row++;
                i_column = 0;
                printf("\n");
            }
           

        }

        
        i_row = 0;
        i_column = 0;

        while (i_row != row+1 && i_column != max_column+1){

            if(table[i_row][i_column][0]=='='){
                printf("found exp at r:%d c:%d \t", i_row, i_column);
                parse_exp(table[i_row][i_column]);
            }
            i_column++;

            if(i_column == max_column+1){
                i_row++;
                i_column = 0;
                printf("\n");
            }

        }

    } 
    else {
        printf("no such file\n");
    }

return 0;
}
