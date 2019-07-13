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
                c_chain->ID = current_chainID;
                c_chain->residues = &p->residues[p->num_residues];
                c_chain->protein = p;
                
            }else if(current_residueID != c_residue->ID){
                p->num_residues++;
                c_residue = &p->residues[p->num_residues];
                
                c_residue->ID = current_residueID;
                sstring = substring(line, 18, 20);
                strcpy(c_residue->name, sstring);
                free(sstring);
                
                c_residue->atoms = &p->atoms[p->num_atoms];
                c_atom->residue = &p->residues[p->num_residues];
                if(current_chainID != c_chain->ID){
                    c_chain->num_residues++;
                    p->num_chains++;
                    c_chain = &p->chains[p->num_chains];
                    c_chain->ID = current_chainID;
                    c_chain->protein = p;
                    c_chain->residues = c_residue;
                }
                c_residue->chain = c_chain;
                c_chain->num_residues++;
            }else{
                c_atom->residue = &p->residues[p->num_residues];
            }
            p->num_atoms++;
            c_residue->num_atoms++;
        }
    }
    c_residue->num_atoms++;
    c_chain->num_residues++;
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

Atom* getAtom(Protein *protein, char chainID, int residueID, char *atomName){
    Atom *currAtom;
    int i;
    for (i = 0; i < protein->num_atoms; i++){
        currAtom = &(protein->atoms[i]);
        if(strcmp(currAtom->name, atomName) == 0 && currAtom->residue->ID == residueID && currAtom->residue->chain->ID == chainID){
            return currAtom;
        }
    }
    printf("not able to find the following atom: %c  %d  %s", chainID, residueID, atomName);
    return NULL;
}

Residue* getResidue(Protein *protein, char chainID, int residueID){
    Residue *currRes;
    int i;
    for(i = 0; i < protein->num_residues; i++){
        currRes = &(protein->residues[i]);
        if(currRes->ID == residueID && currRes->chain->ID == chainID){
            return currRes;
        }
    }
    printf("not able to find the following residue: %c  %d", chainID, residueID);
    return NULL;
}

Chain* getChain(Protein* protein, char chainID){
    Chain *currChain;
    int i;
    for(i = 0; i < protein->num_chains; i++){
        currChain = &(protein->chains[i]);
        if(currChain->ID == chainID){
            return currChain;
        }
    }
    printf("not able to find the following chain: %c", chainID);
    return NULL;
}

double dist(double *a, double *b){
    if(a == NULL || b == NULL){
        return -1;
    }
    
    double sum = 0;
    int i;
    for(i = 0; i < 3; i++){
        sum += pow(a[i] - b[i], 2);
    }
    return sqrt(sum);
}

double distAtom(Atom *a, Atom *b){
    if(a == NULL || b == NULL){
        return -1;
    }
    
    return dist(a->coor, b->coor);
}

double distResidue(Residue *a, Residue *b){
    Atom *curAtomA = a->atoms;
    Atom *curAtomB = b->atoms;
    double dist = 0.0;
    int i, j;
    for(i = 0; i < a->num_atoms; i++){
        for(j = 0; j < b->num_atoms; j++){
            if(dist == 0 || distAtom(curAtomA, curAtomB) < dist){
                dist = distAtom(curAtomA, curAtomB);
            }
            curAtomB++;
        }
        curAtomA++;
        curAtomB = b->atoms;
    }
    
    return dist;
}

double distResidueCA(Residue *a, Residue *b){
    Atom *curAtomA = a->atoms;
    Atom *curAtomB = b->atoms;
    int i, j;
    for(i = 0; i < a->num_atoms; i++){
        for(j = 0; j < b->num_atoms; j++){
            if(strcmp(curAtomA->name," CA ") == 0 && strcmp(curAtomB->name," CA ") == 0){
                return distAtom(curAtomA, curAtomB);
            }
            curAtomB++;
        }
        curAtomA++;
        curAtomB = b->atoms;
    }
    
    return -1;
}

void contacts(Protein *protein, double maxDist, int criteria){
    int i, j, numRes = 0;
    Residue* res = protein->residues;
    printf("chain - resID\tchain - resID\t\tshortest dist\tCA dist\n");
    for(i = 0; i < protein->num_residues - criteria; i++){
        for(j = i + criteria; j < protein->num_residues; j++){
            if (distResidue(&res[i], &res[j]) <= maxDist){
                numRes++;
                printf("%c\t-\t%d\t\t%c\t-\t%d\t\t\t%.5lf\t\t\t%.5lf\n",res[i].chain->ID, res[i].ID, res[j].chain->ID, res[j].ID,distResidue(&res[i] , &res[j]), distResidueCA(&res[i], &res[j]) );
            }
        }
    }
    printf("There are %d residue pairs who interact\n\n",numRes);
}
