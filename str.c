#include"str.h"
#include"common.h"

void str_trim_crlf(char *str)
{
	char *p = &str[strlen(str)-1];
        while(*p == '\r' || *p == '\n')
                *p-- = '\0';

}

void str_split(const char *str, char *left, char *right, char c)
{
	char *p = strchr(str, c);
	if(p == NULL)
		strcpy(left, str);
	else
	{
		strncpy(left, str, p-str);
		strcpy(right, p+1);
	}

}		

int str_all_space(const char *str)
{
	while(*str)
	{
		if(!isspace(*str))
			return 0;
		str++;
	}

	return 1;
}

void str_upper(char *str)
{
	while(*str)
	{
		*str = toupper(*str);
		str++;
	}
}

long long str_to_longlong(char *str)
{
	//return atoll(str);
	long long result = 0;
	long long mult = 1;
	if(str == NULL)
		return 0;
	
	unsigned int len = strlen(str);
	if(len > 15)
		return 0;
	
	//unsigned int i;//i-- shi cuo wu
	int i;
	/*for(i=0; i<len; i++)
	{
		char ch = str[len-(i+1)];
		if(ch > '9' || ch < '0')
			return 0;

		long long val = ch - '0';
		val *= mult;
		result += val;
		mult *=10;
	}*/
	for(i=len-1; i>=0; i--)
	{
		char ch = str[len-(i+1)];
                if(ch > '9' || ch < '0')
                        return 0;

                long long val = ch - '0';
                val *= mult;
                result += val;
                mult *=10;
	}
	return result;
}

//8 jing zhi zheng shu zhuan huan chegn 10 jin zhi zheng shu 
unsigned int str_octal_to_uint(const char *str)
{
	/*if(str == NULL)
		return 0;
	
	unsigned int len = strlen(str);	
	if(len > 10)
		return 0;

	unsigned int result = 0;
	int i;
	for(i=0; i<len; i++)
	{
		char ch = str[i];
		if(ch >'9' || ch < '0')
			return 0;
		
		unsigned int val = ch - '0'; 
		result = result*8 + val;

	}
	return result;
	*/
	if(str == NULL)
		return 0;

	unsigned int result = 0;
	short seen_non_zero_digit = 0;
	
	while(*str)
	{
		int digit = *str;
		if(!isdigit(digit) || digit > '7')
			break;

		if(digit != '0')
			seen_non_zero_digit = 1;
		
		if(seen_non_zero_digit == 1)
		{
			result <<= 3;
			result += (digit-'0');
		}
		str++;
	}

	return result;
}




