#include <u_stdlib/u_string.h>


/****************************************
 *  Функция декодирования числа в формат строки
 * integer - число
 * возвращает массив
 * *********************************/

//---------------------------------------------------------

// void intToStr(int integer,char* baf){
// 	/* Функция декодирования числа в формат строки
// 	integer - число
// 	buf - возвразаемая строка */
// 	char buf[11]={0};
//   	int i=0;
// 	int flag=0;
//     	if(integer<0){
//         	flag=1;
//         	integer*=-1;
//         	i++;
//     	}//if если отрицательное
//     	int b=10;
//     	while(1){
//         	buf[i]=((integer%b)-(integer%(b/10)))/(b/10);
//         	buf[i]+=48;
//         	if(integer%b==integer)
// 			break;
//         	b*=10;
//         	i++;
//     	}//while

// 	int qw=0;
// 	if(flag){i++;buf[i]='-';}
// 	while(i>=0){
// 		baf[qw]=buf[i];
// 		i--;
// 		qw++;
// 	}

//     	baf[qw]='\0';
// 	return;
// }//intToStr

// ----------------------------------------------------------

/* Return the length of S.  */
size_t u_strlen (const char *__s){
	int i=0;
	while(1){
		if(__s[i]=='\0')break;
		i++;
	}
	return i;
}

//----------------------------------------------------------
/*****************************************************
 *  Используется алгоритм Бойера-Мура	             *
 * ------------------------------------------------- */
/* Find the first occurrence of NEEDLE in HAYSTACK.  */
char* u_strstr(const char* __haystack, const char* __needle){
    
    int len = u_strlen(__needle); //длина запроса
    char* end_adress = (char*)(len + (size_t)__needle); //адрес конца
    char* table =(char*) u_malloc(len); //table
    

    //======================
    //--1) Составляем таблицу --
	//======================
    for (int i=1; i<=len;i++){
        
        //-- Присваиваем позицию последнего вхождения
        table[len-i] = end_adress - u_strrchr(__needle, __needle[len-i]);

        //-- исключение завершающий символ:
        if (table[len-i]==0){

			//-- если такой символ есть, то присваеваем ему его индекс
            if(u_strrevchr(__needle+len-1,len-1,__needle[len-i])) 
			{
                table[len-i] = end_adress - u_strrevchr(__needle+len-1,len-1,__needle[len-i]);
			} 
			 //-- иначе равным длинне строки
			 else 
			{
                table[len-i]=len;
			}

		}
    }// = Таблица готова =

    //======================
    //--2) поиск в строке
	//======================
    int counter=len;                  // <- начинаем с конца слова
    int text_len=u_strlen(__haystack);  // инициализация длинны массива, в котором ищем
    char* tmp_res=0;

    while(counter<=text_len){

        //-- Проверяем, нашли ли мы то, что искали
        if(u_strncmp(__haystack+counter-len, __needle, len)==0)
        {   
            u_free (table);
            return (char*)((size_t)__haystack+counter-len);
        }
        //-- если нет, то уточнаем, есть ли такой символ в поисковом запросе
        tmp_res = u_strchr(__needle, __haystack[counter]);
        if(tmp_res){

            //-- если есть, то ищем индекс в таблице, и ищем уже на индексном расстоянии от места поиска
            counter += (table[(tmp_res-__needle)]);
  
            
        }else{
			//-- иначе можно пропустить слово и искать на расстояни len
            counter=counter+len;
        }
        
    }
    //-- если не нашли, то возвращаем указатель на нуль
    u_free (table);
    return 0;
    
}


//-------------------------------------------------------


//обратный поиск
char* u_strrevchr(const char* line, int len , const char chr){
    line= line - len;
    
    int i=len;
    while(i>0){
        if (line[i-len]==chr)return (char*)((size_t)line+i-len);
        i--;
    }
    return 0;
}

//----------------------------------------------------------

/* Find the first occurrence of C in S.  */
char* u_strchr (const char *__s, int __c){
	for(int i=0;i<u_strlen(__s);i++){
		if(__s[i]==__c) return (char*)((size_t)__s + i);
	}
	return 0;
}

//----------------------------------------------------------

/* Find the last occurrence of C in S.  */
char* u_strrchr(const char *__s, int __c){
	for(int i=u_strlen(__s);i>=0;i--){
		if(__s[i]==(char)__c) return (char*)((size_t)__s + i);
	}
	return 0;

}

//-----------------------------------------------------------

/* Compare N characters of S1 and S2.  */
int u_strncmp(const char* __str1, const char* __str2, size_t __n){

	for(int i=0; i < __n; i++)
	{
		if(__str1[i]!=__str2[i] || __str1[i]==0 || __str2[i]==0)
		{
			return (__str1[i] - __str2[i]);
		}

	}
	return 0;

}

// -----------------------------------------------------------------


static void reverse(char s[])
{
	int i, j;
	char c;

	for (i = 0, j = u_strlen(s)-1; i<j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}


 
#ifdef NO_DIVISION
void* u_intToStr(int integer){


	char buf[]={'0','0','0','0','0',   '0','0','0','0','0',  '\0'};
  	int flag_minus=0, 
	  	i=0,
	  	carry=0,
	  	i_max=0;

	  	//для отрицательных чисел нужен "-"
    	if(integer<0)
		{
        	integer*=-1;
        	flag_minus=1;
    	}
		

		// по методу инкрементирования набираем наше число
		// для больших чисел - очень долго!!! но зато без деления
		while(integer)
		{
            (i_max<i)?(i_max=i):(i_max);
			i=0;
			carry=1;

			while (carry&1)
			{
                buf[i]++;
				if(buf[i]==58) {
					buf[i]='0';
					i++;
					carry=1;	
				}else carry = 0;
			}
			integer--;
		}
    
        
		//для отрицательных чисел нужен "-"
        if(flag_minus){
            i_max++;
            buf[i_max]='-';
        }


        char* ret_buf=(char*)u_malloc(i_max+1);

		//разворачиваем массив и убираем лишние нули
        for(int i=0;i<i_max+1;i++){
            ret_buf[i]=buf[i_max-i];
        }

	return ret_buf;

}//itoa

void u_intToHex(unsigned int n, char* s)
{
    int i=0;
    do {       /* генерируем цифры в обратном порядке */
        s[i] = (s8)( ((n & (0x0f<<(i*4)))>>(i*4)) + '0');   /* берем следующую цифру */
        if(s[i]>'9')
        {
            s[i]+=7;
        }
        n &= ~(0x0f<<(i*4));
        i++;
    }while(n);     /* удаляем */

    s[i] = '\0';
    reverse(s);
}

#else
/* itoa:  конвертируем n в символы в s */
void u_intToHex(unsigned int n, char* s)
 {
     int i;
 
     i = 0;
     do {       /* генерируем цифры в обратном порядке */
         s[i] = n % 16 + '0';   /* берем следующую цифру */
         if(s[i]>'9'){s[i]+=7;}
         i++;
     } while ((n /= 16) > 0);     /* удаляем */

     s[i] = '\0';
     reverse(s);
 }


 /* itoa:  конвертируем n в символы в s */
void u_intToStr(unsigned int n, char* s)
 {
     int i;
 
     i = 0;
     do {       /* генерируем цифры в обратном порядке */
         s[i] = n % 10 + '0';   /* берем следующую цифру */
        //  if(s[i]>'9'){s[i]+=7;}
         i++;
     } while ((n /= 10) > 0);     /* удаляем */

     s[i] = '\0';
     reverse(s);
 }




#endif

/**
 * isdigit
 * tolower
 * isspace
 * isxdigit
 * 
 * */
int u_isdigit(char ch)
{
    if(ch>='0'&&ch<='9')
        return 1;
    return 0;
}
int u_isxdigit(char ch)
{
    if(u_isdigit(ch)&&
    ((ch<='F')&& (ch>='A'))&&
    ((ch<='f')&& (ch>='a')))
        return 1;
    return 0;
}

int u_isupper(char ch)
{
    if(ch>='A'&&ch<='Z')
        return 1;
    return 0;
}

char u_tolower(char ch)
{
    if(u_isupper(ch))
        return(ch+32);
    return ch;
}

int u_isspace(char ch)
{
    switch (ch)
    {
    case ' ':
    case '\t':
    case '\n':
    case '\r':
       return 1;
    default:
        return 0;
    }
}
//===========================================================



/* Чёрная магия с просторов хабра, красиво, как по мне
u8 CountOnes3 (u8 n) {
  if (n == 0)  return 0;  // Единственный случай, когда ответ 0.
  if (n == 0xFF)  return 8;  // Единственный случай, когда ответ 8.
  n = (0x010101*n & 0x249249) % 7;  // Считаем число бит по модулю 7.
  if (n == 0)  return 7;  // Гарантированно имеем 7 единичных битов.
  return n;  // Случай, когда в числе от 1 до 6 единичных битов.
}
*/
