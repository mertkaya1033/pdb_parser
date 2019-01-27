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
    Protein* p = (Protein*) malloc(sizeof(Protein));
    p->num_atoms = 0;
    p->num_residues = 0;
    p->num_chains = 0;
    p->atoms = (Atom*) malloc(100000 * sizeof(Atom));
    p->residues =(Residue*) malloc(70000 * sizeof(Residue));
    p->chains = (Chain*) malloc(100 * sizeof(Chain));
    
    Atom* c_atom = NULL;
    Residue* c_residue = NULL;
    Chain* c_chain = NULL;
    int current_residueID = 0;
    char current_chainID = ' ';
    char line[82];
    char* sstring;
    FILE* file = fopen(file_name, "r");
    while(!feof(file)){
        fgets(line, 82, file);
        if(strncmp(line, "ATOM  ", 6) == 0){
            c_atom = &p->atoms[p->num_atoms];
            c_residue = &p->residues[p->num_residues];
            c_chain = &p->chains[p->num_chains];
            
            //atom init
            sstring = substring(line, 7, 11);
            c_atom->ID = atoi(sstring);
            free(sstring);
            
            sstring = substring(line, 13, 16);
            strcpy(c_atom->name, sstring);
            free(sstring);
            
            sstring = substring(line, 31, 38);
            c_atom->coor[0] = atof(sstring);
            free(sstring);
            
            sstring = substring(line, 39, 46);
            c_atom->coor[1] = atof(sstring);
            free(sstring);
            
            sstring = substring(line, 47, 54);
            c_atom->coor[2] = atof(sstring);
            free(sstring);
            
            //current ID initialization
            sstring = substring(line, 23, 26);
            current_residueID = atoi(sstring);
            free(sstring);
            
            current_chainID = line[21];//22nd character
            
            if(p->num_atoms == 0){ //if it is the first atom in the file
                c_atom->residue = &p->residues[p->num_residues];
                
                c_residue->ID = current_residueID;
                sstring = substring(line, 18, 20);
                strcpy(c_residue->name, sstring);
                free(sstring);
                c_residue->atoms = &p->atoms[p->num_atoms];
                c_residue->chain = &p->chains[p->num_chains];
                c_residue->num_atoms++;
                
                c_chain->ID = current_chainID;
                c_chain->residues = &p->residues[p->num_residues];
                c_chain->protein = p;
                
            }else if(current_residueID != c_residue->ID){
                c_residue->num_atoms++;
                p->num_residues++;
                c_residue = &p->residues[p->num_residues];
                
                c_residue->ID = current_residueID;
                sstring = substring(line, 18, 20);
                strcpy(c_residue->name, sstring);
                free(sstring);
                
                c_residue->atoms = &p->atoms[p->num_atoms];
                c_residue->num_atoms++;
                
                if(current_chainID != c_chain->ID){
                    c_chain->num_residues++;
                    p->num_chains++;
                    c_chain = &p->chains[p->num_chains];
                    c_chain->ID = current_chainID;
                    c_residue->chain = c_chain;
                    c_chain->protein = p;
                    c_chain->residues = c_residue;
                    c_chain->num_residues++;
                }
            }
            p->num_atoms++;
        }
    }
    p->num_residues++;
    p->num_chains++;
    return p;
};
char* substring(char* string, int start, int end){
    char* sub_string = (char*) malloc((end - start + 2) * sizeof(char));
    int i, j;
    for(i = start - 1, j = 0; i < end; i++, j++){
        sub_string[j] = string[i];
    }
    sub_string[end - start + 1] = '\0';
    return sub_string;
}

void free_protein(Protein* p){
    free(p->atoms);
    free(p->residues);
    free(p->chains);
    free(p);
}
