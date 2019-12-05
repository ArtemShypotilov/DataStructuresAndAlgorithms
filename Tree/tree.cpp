#define NAME_MAXLEN 6
#define PATH_MAXLEN 1999
#define MAX_NUM_DIR 50000
 
typedef struct _dir {
    int id;
    int name;
    int parent;
    int child;
    int prev;
    int next;
}Dir;
 
Dir dirs[MAX_NUM_DIR];
int c_dir;
Dir* root;
 
Dir* allocDir() {
    dirs[c_dir].id = c_dir;
    dirs[c_dir].parent = -1;
    dirs[c_dir].child = -1;
    dirs[c_dir].prev = -1;
    dirs[c_dir].next = -1;
    return &dirs[c_dir++];
}
 
void init(int n) {
    c_dir = 0;
    root = allocDir();
    root->name = 0;
}
 
Dir* search(char* path) {
    Dir* d = root;
    register unsigned int name = 0;
    register int i = 1;
 
    while (path[i]) {
        if (path[i] == '/') {
            if (d->child) {
                Dir * c = &dirs[d->child];
                while (1) {
                    if (c->name == name) {
                        d = c;
                        name = 0;
                        break;
                    }
                    c = &dirs[c->next];
                }
            }
        }
        else {
            name <<= 5;
            name |= (path[i] - 'a' + 1);
        }
        i++;
    }
    return d;
}
 
void addDir(Dir* dst, Dir* src) {
    src->parent = dst->id;
    if (dst->child == -1) {
        dst->child = src->id;
        src->parent = dst->id;
    }
    else {
        Dir* sub = &dirs[dst->child];
        while (sub->next != -1) {
            sub = &dirs[sub->next];
        }
        sub->next = src->id;
        src->prev = sub->id;
    }
}
 
void cmd_mkdir(char path[PATH_MAXLEN + 1], char name[NAME_MAXLEN + 1]) {
    Dir* last = search(path);
    register unsigned int n = 0;
    register int i = 0;
 
    while (name[i]) {
        n <<= 5;
        n |= (name[i] - 'a' + 1);
        i++;
    }
 
    Dir* nd = allocDir();
    nd->name = n;
    addDir(last, nd);
}
 
void cmd_rm(char path[PATH_MAXLEN + 1]) {
    Dir* tar = search(path);
    Dir* p = &dirs[tar->parent];
    Dir* c = &dirs[p->child];
 
    while (1) {
        if (c->name == tar->name) {
            c->parent = -1;
            if (c->prev == -1) {
                p->child = c->next;
                dirs[c->next].prev = -1;
            }
            else {
                dirs[c->prev].next = c->next;
                dirs[c->next].prev = c->prev;
            }
            break;
        }
        c = &dirs[c->next];
    }
}
 
void dsp_cp(Dir* dst, Dir* src) {
    Dir* nd = allocDir();
    nd->name = src->name;
    addDir(dst, nd);
 
    if (src->child != -1) {
        Dir* c = &dirs[src->child];
        while (1) {
            dsp_cp(nd, c);
            if (c->next == -1) break;
            c = &dirs[c->next];
        }
    }
}
 
void cmd_cp(char srcPath[PATH_MAXLEN + 1], char dstPath[PATH_MAXLEN + 1]) {
    Dir* src = search(srcPath);
    Dir* dst = search(dstPath);
 
    dsp_cp(dst, src);
}
 
void cmd_mv(char srcPath[PATH_MAXLEN + 1], char dstPath[PATH_MAXLEN + 1]) {
    Dir* src = search(srcPath);
    Dir* dst = search(dstPath);
    Dir* p = &dirs[src->parent];
    Dir* c = &dirs[p->child];
 
    while (1) {
        if (c->name == src->name) {
            if (c->prev == -1) {
                dirs[c->parent].child = c->next;
                dirs[c->next].prev = -1;
            }
            else {
                dirs[c->prev].next = c->next;
                dirs[c->next].prev = c->prev;
            }
            c->prev = -1;
            c->parent = -1;
            c->next = -1;
            break;
        }
        if (c->next == -1)
            break;
        c = &dirs[c->next];
    }
 
    addDir(dst, src);
}
 
int getNum(Dir* tar) {
    register int count = 0;
 
    if (tar->child != -1) {
        Dir* c = &dirs[tar->child];
        while (1) {
            count += getNum(c) + 1;
            if (c->next == -1) break;
            c = &dirs[c->next];
        }
    }
    return count;
}
 
int cmd_find(char path[PATH_MAXLEN + 1]) {
    register int count = 0;
    Dir* tar = search(path);
    if (tar->child != -1) {
        Dir* c = &dirs[tar->child];
        while (1) {
            count += getNum(c) + 1;
            if (c->next == -1) break;
            c = &dirs[c->next];
        }
    }
    return count;
}
