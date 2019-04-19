	#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
typedef struct Fields
{
	char * field_name;
	int data_type;//integer representing the datatype.
}Fields;
typedef struct datatype //structure to store various datatypes
{
	int int_value;
	char* string;
	float float_value;

}datatype;


typedef struct table
{
	int rows, cols;
	struct Fields* headers;
	char * schema_name;
	struct datatype*** cell;
}table;

typedef struct Query // storing the query in the form of a structure
{

	char* cmd;
	char *opr;        //contains which operation to be performed
	int  *display;   // array which contains which feilds are to be displayed
	int  source;     //contains on which feild the operation is to be performed
	char* dest;		 //contains the constant
	int sh;

}query;

void display(query*q, table*database, int row);
void getcmd(char * token, query *q1);
int get_schema(char*token, query* q1, int *index, table*database);
void getsdisp(char*buffer1, query*q1, int index, table*database);
void getsrc(char* source, query* q1, int index, table*database);
void getdest(char* dest1, query *q1);
void getoper(char* token, query *q1);
void execute_query(char *cmd, table*database, int* index, query* q1);
int subString(char* str, char*substr);
void str_operation(char * temp, query* q, table*database, int row);
void operation(float temp, query*q, table*database, int row);
void implement(query* q, table*database);
void execute(char *cmd, table*database, int* index);

void getcmd(char * token, query *q1)
{
	q1->cmd = (char*)malloc(10 * sizeof(char));
	strcpy(q1->cmd, token);
}
int get_schema(char*token, query* q1, int *index, table*database)
{
	int i;
	for (i = 0; i < *index; i++)
		if (strcmp(database[i].schema_name, token) == 0)
			break;
	q1->sh = i;
	return i;
}
void getsdisp(char*buffer1, query*q1, int index, table*database) // to retrieve the feilds that are to be displayed and storing them in the query..
{
	int cols = database[index].cols;
	int* arr = (int*)calloc(cols, sizeof(int));
	int i = 0;
	char display1[25];
	char*buffer = (char*)malloc(10);
	strcpy(buffer, buffer1);


	while (buffer[i] != '\0')
	{
		int j = 0;
		while (buffer[i] != ',' && buffer[i] != '\0' &&buffer[i] != '\n')
			display1[j++] = buffer[i++];
		display1[j] = '\0';
		if (buffer[i] != '\0')
			i++;
		if (strcmp("*", display1) == 0)
		{
			for (int i = 0; i < cols; i++)
				arr[i] = 1;
		}
		for (int i = 0; i < cols; i++)
		{
			if (strcmp(display1, database[index].headers[i].field_name) == 0)
				arr[i] = 1;
		}
	}
	q1->display = arr;
}
void getsrc(char* source, query* q1, int index, table*database) // to get feild in the table on which operation is to be performed.
{
	int i = 0;
	while (strcmp(source, database[index].headers[i].field_name) != 0)
		i++;
	q1->source = i;
}
void getdest(char* dest1, query *q1) // to get the comparion constant operator
{
	q1->dest = (char*)malloc(20 * sizeof(char));
	strcpy(q1->dest, dest1);
}
void getoper(char* token, query *q1)
{
	q1->opr = (char*)malloc(100 * sizeof(char));
	strcpy(q1->opr, token);
}
void execute_query(char *cmd, table*database, int* index, query* q1) // to execute the query
{
	int count = 0, id;
	char *token = strtok(cmd, " ");
	char*table_name = (char*)malloc(50 * sizeof(char));
	while (token != NULL)
	{
		if (count == 0)
			getcmd(token, q1);
		if (count == 1)
		{
			strcpy(table_name, token);
		}
		if (count == 3)
			id = get_schema(token, q1, index, database);

		if (count == 5)
			getsrc(token, q1, id, database);
		if (count == 6)
			getoper(token, q1);
		if (count == 7)
			getdest(token, q1);
		token = strtok(NULL, " ");
		count++;

	}
	if (count == 4)
		q1->source = -1;
	getsdisp(table_name, q1, id, database);
}
int subString(char* str, char*substr)// fucntion to check if the given string contains a substring
{
	int count1 = 0, count2 = 0, i, j, flag = 0;
	while (str[count1] != '\0')
		count1++;
	while (substr[count2] != '\0')
		count2++;
	for (i = 0; i <= count1 - count2; i++)
	{
		for (j = i; j<i + count2; j++)
		{
			flag = 1;
			if (str[j] != substr[j - i])
			{
				flag = 0;
				break;
			}
		}
		if (flag == 1)
			break;
	}
	if (flag == 1)
		return 1;
	else
		return 0;
}
void str_operation(char * temp, query* q, table*database, int row) // performs contains and matches operations on the string
{
	if (strcmp(q->opr, "contains") == 0)
	{
		if (subString(temp, q->dest))
			display(q, database, row);
	}
	else if (strcmp(q->opr, "matches") == 0)
	{
		if (strcmp(temp, q->dest) == 0)
			display(q, database, row);
	}
}
void operation(float temp, query*q, table*database, int row) //to perform  the operation lessthan,greaterthan,equals,lessthan or equals,greater than or equals
{
	if (strcmp(q->opr, "LT") == 0)
	{
		if (temp < atoi(q->dest))
			display(q, database, row);
	}
	if (strcmp(q->opr, "GT") == 0)
	{
		if (temp >atoi(q->dest))
			display(q, database, row);

	}
	if (strcmp(q->opr, "GTE") == 0)
	{
		if (temp >= atoi(q->dest))
			display(q, database, row);
	}
	if (strcmp(q->opr, "LTE") == 0)
	{
		if (temp <= atoi(q->dest))
			display(q, database, row);
	}
	if (strcmp(q->opr, "EQ") == 0)
	{
		if (temp == atoi(q->dest))
			display(q, database, row);
	}

}
void display(query*q, table*database, int row) // to print the result on to the standard output.
{
	for (int i = 0; i < database[q->sh].cols; i++)
	{
		if (q->display[i] == 1)
		{
			if (database[q->sh].headers[i].data_type == 1)
				printf("%3d  ", database[q->sh].cell[row][i]-> int_value);
			if (database[q->sh].headers[i].data_type == 2)
				printf("%10s ", database[q->sh].cell[row][i]-> string);
			if (database[q->sh].headers[i].data_type == 3)
				printf("%3f  ", database[q->sh].cell[row][i]-> float_value);
		}
	}
	printf("\n");
}
void implement(query* q, table*database) 
{
	char * temp = (char*)malloc(20 * sizeof(char));
	if (strcmp(q->cmd, "select") == 0)
	{

		for (int i = 0; i <database[q->sh].rows; i++)
		{
			if (q->source == -1)
				display(q, database, i);
			else
			{
				if (database[q->sh].headers[q->source].data_type == 1)
				{
					operation(database[q->sh].cell[i][q->source]-> int_value, q, database, i);
				}
				if (database[q->sh].headers[q->source].data_type == 2)
				{
					str_operation(database[q->sh].cell[i][q->source]-> string, q, database, i);
				}
				if (database[q->sh].headers[q->source].data_type == 3)
				{
					operation(database[q->sh].cell[i][q->source]-> float_value, q, database, i);
				}
			}

		}
	}
	else
		printf("\ninvalid command\n");
}
void flush(char*buffer, table*database,int *index)
{
	int k = 0;
	char*token = strtok(buffer, " ");
	token = strtok(NULL, " ");
	FILE* fp1 = fopen(token, "w");
	token = strtok(NULL, " ");
	char * name = (char*)malloc(10 * sizeof(char));
	strcpy(name, token);
	while (strcmp(database[k].schema_name, name) != 0)
		k++;
	for (int i = 0; i < database[k].rows; i++)
	{
		for (int j = 0; j < database[k].cols; j++)
		{
			if (database[k].headers[j].data_type == 1)
			{
				fprintf(fp1, "%d,", database[k].cell[i][j]-> int_value);
			}
			if (database[k].headers[j].data_type == 2)
			{
				fprintf(fp1, "%s,", database[k].cell[i][j]-> string);

			}
			if (database[k].headers[j].data_type == 3)
			{
				fprintf(fp1, "%f,", database[k].cell[i][j]-> float_value);
			}
		}
		fprintf(fp1, "\n");
	}
	fclose(fp1);
}
void create(char* buffer,table*database,int*index)
{
	char** feilds = (char**)malloc(15 * sizeof(char*));
	for (int i = 0; i <15; i++)
		feilds[i] = (char*)malloc(50 * sizeof(char));
	char** datatypes = (char**)malloc(15 * sizeof(char*));
	for (int i = 0; i <15; i++)
		datatypes[i] = (char*)malloc(50 * sizeof(char));
	char* token = strtok(buffer, " ");
	token = strtok(NULL, " ");
	database[*index].schema_name = (char*)malloc(10 * sizeof(char));
	strcpy(database[*index].schema_name, token);

	int len = 0;
	while (token != NULL)
	{
		token = strtok(NULL, " ");
		int i = 0, j = 0;
		if (token == NULL)
			break;
		while (token[i] != '\0' && token[i] != ':')
		{
			feilds[len][j++] = token[i++];
		}
		feilds[len][j] = '\0';
		j = 0;
		i++;
		if (token == NULL)
			break;
		while (token[i] != '\0' && token[i] != ':')
		{
			datatypes[len][j++] = token[i++];
		}
		datatypes[len][j] = '\0';
		len++;
		database[*index].cols = len;
	}
	Fields* f = (Fields*)malloc(len*sizeof(Fields));
	for (int i = 0; i < len; i++)
	{
		f[i].field_name = (char*)malloc(10 * sizeof(char));
		strcpy(f[i].field_name, feilds[i]);
		if (strcmp("int", datatypes[i]) == 0)
			f[i].data_type = 1;
		if (strcmp("string", datatypes[i]) == 0)
			f[i].data_type = 2;
		if (strcmp("float", datatypes[i]) == 0)
			f[i].data_type = 3;
	}
	database[*index].headers = f;
	*index = *index + 1;
	free(datatypes);
}
void import(char* buffer1,table*database,int*index)
{
	int k = 0;
	char *token = strtok(buffer1, " ");
	token = strtok(NULL, " ");
	FILE* fp1 = fopen(token, "r");
	char * name = (char*)malloc(10 * sizeof(char));
	token = strtok(NULL, " ");
	strcpy(name, token);
	char buffer[200];
	int l = 0;
	while (fgets(buffer, 250, fp1))
		l++;
	datatype *** temp = (datatype***)malloc(l*sizeof(datatype**));
	rewind(fp1);
	int i = 0;

	while (strcmp(database[k].schema_name, name) != 0)
		k++;
	while (fgets(buffer, 250, fp1))
	{
		temp[i] = (datatype**)malloc(sizeof(datatype*));
		int j = 0;
		token = strtok(buffer, ",");

		while (token != NULL)
		{
			temp[i][j] = (datatype*)malloc(sizeof(datatype));
			if (database[k].headers[j].data_type == 1)
			{
				temp[i][j]-> int_value = atoi(token);
			}
			if (database[k].headers[j].data_type == 2)
			{
				temp[i][j]-> string = (char*)malloc(10 * sizeof(char));
				strcpy(temp[i][j]-> string, token);

			}
			if (database[k].headers[j].data_type == 3)
			{
				temp[i][j]-> float_value = atoi(token);
			}
			j++;
			token = strtok(NULL, ",");
		}
		i++;
	}
	database[k].schema_name = (char*)malloc(10 * sizeof(char));
	strcpy(database[k].schema_name, name);
	database[k].cell = temp;
	database[k].rows = l;
}
int check_primarykey(table* database,int index1,int index2,int row1,int row2)// to check whether the primary keys of one row of table1 and another rows of table2
{
	if (database[index1].headers[0].data_type==1)
	{
		if (database[index1].cell[row1][0]-> int_value == database[index2].cell[row2][0]-> int_value)
			return 1;
		else
			return 0;
	}
	if (database[index1].headers[0].data_type == 2)
	{
		if (strcmp(database[index1].cell[row1][0]-> string, database[index2].cell[row2][0]-> string) == 0)
			return 1;
		else
			return 0;
	}
	if (database[index1].headers[0].data_type == 3)
	{
		if (database[index1].cell[row1][0]-> float_value == database[index2].cell[row2][0]-> float_value)
			return 1;
		else
			return 0;
	}
}
void join(char* buffer,table*database,int*index,datatype*** temp1)
{
	char*token = (char*)malloc(10 * sizeof(char*));
	token = strtok(buffer, " ");
	char*s1 = (char*)malloc(20 * sizeof(char));
	s1 = strtok(NULL, " ");
	char*s2 = (char*)malloc(20 * sizeof(char));
	s2 = strtok(NULL, " ");
	char*s3 = (char*)malloc(20 * sizeof(char));
	s3 = strtok(NULL, " ");
	int index1 = 0, index2 = 0;
	while (strcmp(database[index1].schema_name, s1) != 0)
		index1++;
	while (strcmp(database[index2].schema_name, s2) != 0)
		index2++;
	database[*index].schema_name = (char*)malloc(10 * sizeof(char));
	strcpy(database[*index].schema_name, s3);
	database[*index].cols = database[index1].cols + database[index2].cols - 1;
	Fields* f = (Fields*)malloc((database[*index].cols)*sizeof(Fields));
	int i=0,j = 0, k = 0, l = 0, m = 0;
	for (int i = 0; i <database[index1].cols; i++, j++)
		f[j] = database[index1].headers[i];
	for (int i = 1; i <database[*index].cols; i++, j++)
		f[j] = database[index2].headers[i];
	database[*index].headers = f;
	for (i = 0; i < database[index1].rows; i++)
	{
		for (j = 0; j < database[index2].rows; j++)
		{
				if (check_primarykey(database,index1,index2,i,j))//checking the primary key of two rows...
				{
					l = 0;
					m++;
	
					for (k = 0; k < database[index1].cols; k++, l++)
					{
						temp1[i][l] = database[index1].cell[i][k];
					}
					for (k = 1; k < database[index2].cols; k++, l++)
					{
						temp1[i][l] = database[index2].cell[i][k];
					}

				}

		}

	}
	database[*index].cell = temp1;
	database[*index].rows = m;
	*index = *index + 1;
}
void execute(char *cmd, table*database, int* index)
{
	int len = 0;
	char*cmd1 = (char*)malloc(100 * sizeof(char));
	strcpy(cmd1, cmd);
	char* token = (char*)malloc(15 * sizeof(char));
	token = strtok(cmd, " ");
	if (strcmp(token, "create") == 0)
	{
		create(cmd1, database, index);
	}
	else if (strcmp(token, "import") == 0)
	{
		import(cmd1, database, index);
	}
	else if (strcmp(token, "flush") == 0)
	{
		flush(cmd1, database, index);
	}
	else if (strcmp(token, "join") == 0)
	{
		datatype*** temp1 = (datatype***)malloc(200*sizeof(datatype**));
		for (int i = 0; i < 200; i++)
			temp1[i] = (datatype**)malloc(sizeof(datatype*));
		join(cmd1,database,index,temp1);
	}
	else if (strcmp(token, "select") == 0)
	{
		query *q1 = (query*)malloc(sizeof(query));
		execute_query(cmd1, database, index, q1);
		implement(q1, database);
	}
}
int main()
{
	int index = 0;//index for pointing to a particular table
	table*database = (table*)malloc(10 * sizeof(table*));// array of table where each table will store
	char* cmd = (char*)malloc(100 * sizeof(char));
	Fields** f = (Fields**)malloc(10 * sizeof(Fields*));;
	while (1)
	{
		printf("SQL>>>");
		fflush(stdin);
		gets(cmd);
		execute(cmd, database, &index);
	}
	system("pause");
	return 0;
}
}
int main()
{
	int index = 0;//index for pointing to a particular table
	table*database = (table*)malloc(10 * sizeof(table*));// array of table where each table will store
	char* cmd = (char*)malloc(100 * sizeof(char));
	Fields** f = (Fields**)malloc(10 * sizeof(Fields*));;
	while (1)
	{
		printf("SQL>>>");
		fflush(stdin);
		gets(cmd);
		execute(cmd, database, &index);
	}
	system("pause");
	return 0;
}
