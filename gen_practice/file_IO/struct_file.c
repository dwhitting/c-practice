#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int struct_file_write(void) {

    struct agent {
        char actor[32];
        int year;
        char title[32];
    };
    struct agent bond[3];
    FILE *jbdb;

    jbdb = fopen("bond.db", "w");
    if (!jbdb) {
        puts("SPECTRE wins");
        return 1;
    }

    strcpy(bond[0].actor, "Sean Connery");
    bond[0].year = 1962;
    strcpy(bond[0].title, "Dr. No");  

    strcpy(bond[1].actor, "Roger Moore");
    bond[1].year = 1973;
    strcpy(bond[1].title, "Live and Let Die");
    
    strcpy(bond[2].actor, "Pierce Brosnan");
    bond[2].year = 1995;
    strcpy(bond[2].title, "GoldenEye");

    //for (int x = 0; x < 3; x++) {
        fwrite(&bond, sizeof(struct agent), 3, jbdb);
    //}


    fclose(jbdb);
    puts("Records written");

    return 0;
}

int struct_file_read(void) {

    struct agent {
        char actor[32];
        int year;
        char title[32];
    };
    struct agent bond;
    FILE *jbdb;
    int r;

    jbdb = fopen("bond.db", "r");
    if (!jbdb) {
        puts("SPECTRE wins");
        return 1;
    }
    while (!feof(jbdb)) {
        r = fread(&bond, sizeof(struct agent), 1, jbdb);
        if (r == 0)
            break;
        printf("%s\t%d\t%s\n", bond.actor, bond.year, bond.title);
    }
    fclose(jbdb);

    return 0;
}

int seek_rec(void) {
    struct agent {
        char actor[32];
        int year;
        char title[32];
    };
    struct agent bond;
    FILE *jbdb;

    jbdb = fopen("bond.db", "r");
    if (!jbdb) {
        puts("SPECTRE wins");
        return 1;
    }

    fseek(jbdb, sizeof(struct agent)*2, SEEK_SET);
    fread(&bond, sizeof(struct agent), 1, jbdb);
    printf("%s\t%d\t%s\n", bond.actor, bond.year, bond.title);
    fclose(jbdb);

    return 0;
}