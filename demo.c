#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "sqlite3.h"

#define BUFFER_SIZE 1024

char *trim(char *str);

int clearDatabase(sqlite3 *db){
	if(!db)
		return -1;
	
	char sql[]="delete from words where 1 = 1";
	
	int rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
	if(rc != SQLITE_OK){
		printf("clear SQL error\n");
	}else{
		printf("Clear WORDS COMPLETE\n");
	}
	
	return rc;
}

int addWordData(sqlite3 *db,char *word,char *translate){
	if(!db)
		return -1;
		
	char sql[2 * BUFFER_SIZE];
	
	word = trim(word);
	translate = trim(translate);
	sprintf(sql,"insert into words(word,translate)values('%s','%s');",
			word, translate);
	
	
	char *zErrMsg = 0;
	//printf("run sql : %s\n",sql);
	int rc = sqlite3_exec(db, sql,NULL ,0 ,&zErrMsg);
	if( rc != SQLITE_OK ){
		printf("INSERT SQL error %s\n",zErrMsg);
	}else{
		printf("insert %s , %s successfully\n",word,translate);
	}
	
	return rc;
}

int main(int argc,char *agrv[]){
	char filename[] = "data.txt";
	FILE *fp;
	fp = fopen(filename,"r");
	if(!fp){
		printf("open file %s error\n",filename);
		return -1;
	}
	
	sqlite3 *db;
	char *zErrMsg = NULL;
	int rc;
	
	rc = sqlite3_open("words.db", &db);
	
	if(rc){
		printf("open database words.db error %s\n",sqlite3_errmsg(db));
		fclose(fp);
		return -1;
	}
	
	printf("Opened database successfully\n");
	
	clearDatabase(db);
	
	while(!feof(fp)){
		char word[BUFFER_SIZE];
		char translate[BUFFER_SIZE];
		
		fgets(word,BUFFER_SIZE,fp);
		//printf("%s\n", word); //word
		
		fgets(translate,BUFFER_SIZE,fp);
		//printf("%s\n", translate); //traslate
		
		addWordData(db,word,translate);
	}//end while
	
	fclose(fp);
	
	sqlite3_close(db);
	
	return 0;
}

char *trim(char *str){
	char *end;
	// Trim leading space
	while(isspace((unsigned char)*str)) str++;

	if(*str == 0)  // All spaces?
		return str;

	// Trim trailing space
	end = str + strlen(str) - 1;
	while(end > str && isspace((unsigned char)*end)) end--;

	// Write new null terminator
	*(end+1) = 0;

	return str;
}

