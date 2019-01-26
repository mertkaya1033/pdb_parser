//
//  pdb_parse.c
//  pdb_parse
//
//  Created by Mert Kaya on 2019-01-25.
//  Copyright © 2019 Mert Kaya. All rights reserved.
//

#include "pdb_parse.h"

char* substring(char* string, int start, int end);

Protein* new_protein(char *file_name){
    Protein* newP = (Protein*) malloc(sizeof(Protein));
    newP->num_atoms = 0;
    newP->num_residues = 0;
    newP->num_chains = 0;
    newP->atoms = (Atom*) malloc(100000 * sizeof(Atom));
    newP->residues =(Residue*) malloc(70000 * sizeof(Residue));
    newP->chains = (Chain*) malloc(100 * sizeof(Chain));
    
    int current_residueID = 0;
    char current_chainID = ' ';
    char line[82];
    char* sstring;
    FILE* file = fopen(file_name, "r");
    while(!feof(file)){
        fgets(line, 82, file);
        if(strncmp(line, "ATOM ", 6)){
            sstring = substring(line, 7, 11);
            newP->atoms[newP->num_atoms].ID = atoi(sstring);
            free(sstring);
            
            sstring = substring(line, 13, 16);
            strcpy(newP->atoms[newP->num_atoms].name, sstring);
            free(sstring);
            
            sstring = substring(line, 31, 38);
            newP->atoms[newP->num_atoms].coor[0] = atof(sstring);
            free(sstring);
            
            sstring = substring(line, 39, 46);
            newP->atoms[newP->num_atoms].coor[1] = atof(sstring);
            free(sstring);
            
            sstring = substring(line, 47, 54);
            newP->atoms[newP->num_atoms].coor[2] = atof(sstring);
            free(sstring);
            
            //TODO initialize residues and chains ehe
        }
    }
    
    return newP;
};
char* substring(char* string, int start, int end){
    char* sub_string = (char*) malloc((end - start + 1) * sizeof(char));
    int i, j;
    for(i = start - 1, j = 0; i < end; i++, j++){
        sub_string[j] = string[i];
    }
    sub_string[end - start] = '\0';
    return sub_string;
}
