#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* Global constants */

#define MAIN            "."
#define CSSPATH         "../css/style.css"
#define COOKIE          "HTTP_COOKIE"
#define CLEN            "CONTENT_LENGTH"
#define HTML            "text/html"
#define INVALID_LOGIN   "invalid_login"

/* Global variables */

int indent_sz = 4;
char* indent = "    ";

char* TAG(char* name, int argc, int contc, int close, int ilev, ...)
{
    va_list vl;
    char* res;
    char* str;
    char* argvl[argc];
    char* argvr[argc];
    char* contv[contc];
    int lname;
    int largvl;
    int largvr;
    int lcontv;
    int pos;
    int len;
    int n;
    int i;

    /* Compute result string length */

    lname = strlen(name);

    n = 2 * argc + contc;
    va_start(vl, n);

    for (i = 0; i < argc; ++i)
    {
        str = va_arg(vl, char*);
        argvl[i] = str;

        str = va_arg(vl, char*);
        argvr[i] = str;
    }

    for (i = 0; i < contc; ++i)
    {
        str = va_arg(vl, char*);
        contv[i] = str;
    }

    largvl = 0;
    largvr = 0;
    for (i = 0; i < argc; ++i)
    {
        largvl += strlen(argvl[i]);
        largvr += strlen(argvr[i]);
    }

    lcontv = 0;
    for (i = 0; i < contc; ++i)
        lcontv += strlen(contv[i]);

    va_end(vl);

    /* Allocate memory for result string */

    res = malloc(
        indent_sz * ilev + 1 +          // Indentation
        lname + 2 +                     // Tag name
        largvl + largvr + argc * 4 +    // Arguments
        lcontv +                        // Content
        (close ? lname + 3 +            // Closing tag
        indent_sz * ilev + 1 : 0) +     // Indentation
        1);                             // Trailing zero

    /* Construct result string */

    pos = 0;
    for (i = 0; i < ilev; ++i)
    {
        memcpy(res + pos, indent, indent_sz);
        pos += indent_sz;
    }
    memcpy(res + pos, "<", 1);
    ++pos;

    memcpy(res + pos, name, lname);
    pos += lname;

    for (i = 0; i < argc; ++i)
    {
        memcpy(res + pos, " ", 1);
        ++pos;
        len = strlen(argvl[i]);
        memcpy(res + pos, argvl[i], len);
        pos += len;
        memcpy(res + pos, "=\"", 2);
        pos += 2;
        len = strlen(argvr[i]);
        memcpy(res + pos, argvr[i], len);
        pos += len;
        memcpy(res + pos, "\"", 1);
        ++pos;
    }
    memcpy(res + pos, ">\n", 2);
    pos += 2;

    for (i = 0; i < contc; ++i)
    {
        len = strlen(contv[i]);
        memcpy(res + pos, contv[i], len);
        pos += len;
    }

    if (close != 0)
    {


        for (i = 0; i < ilev; ++i)
        {
            memcpy(res + pos, indent, indent_sz);
            pos += indent_sz;
        }



        memcpy(res + pos, "</", 2);
        pos += 2;
        memcpy(res + pos, name, lname);
        pos += lname;
        memcpy(res + pos, ">\n", 2);
        pos += 2;
    }

    return res;
}

/* Macros */

char* C(char* str, int ilev)
{
    int i;
    int pos;
    int len;
    char* res;

    pos = 0;
    res = malloc(strlen(str) + ilev * indent_sz + 1);
    for (i = 0; i < ilev; ++i)
    {
        memcpy(res + pos, indent, indent_sz);
        pos += indent_sz;
    }

    len = strlen(str);
    memcpy(res + pos, str, len);
    pos += len;

    memcpy(res + pos, "\n", 1);

    return res;
}

char* TITLE(char* str, int ilev)
{
    return TAG("title", 0, 1, 1, ilev, C(str, ilev + 1));
}

char* CSS(char* str, int ilev)
{
    return TAG("link", 2, 0, 0, ilev, "rel", "stylesheet", "href", str);
}

char* H1(char* str, int ilev)
{
    return TAG("h1", 0, 1, 1, ilev, C(str, ilev + 1));
}

char* P(char* str, int ilev)
{
    return TAG("p", 0, 1, 1, ilev, str);
}

char* HEAD(char* title)
{
    return TAG("head", 0, 3, 1, 0,
        TITLE(title, 1),
        CSS(CSSPATH, 1),
        TAG("script", 1, 0, 1, 1, "src", "//html5shim.googlecode.com/svn/trunk/html5.js")
    );
}

/* Code generation */

void CTYPE(char* type)
{
    char* c_type;

    c_type = malloc(15 + strlen(type));
    sprintf(c_type, "Content-Type: %s", type);

    puts(c_type);
}

void LOCATION(char* loc)
{
    char* location;

    location = malloc(12 + strlen(loc));
    sprintf(location, "Location: %s\n", loc);

    puts(location);
}

void SETCOOKIE(char* value, int valid)
{
    char* cookie;

    cookie = malloc(13 + strlen(value) + (valid ? 0 : 11));
    sprintf(cookie, "Set-Cookie: %s%s", value, valid ? "" : "; Max-Age=0");

    puts(cookie);
}

void HTML5()
{
    puts("\n<!DOCTYPE html>");
}

void LOGIN(char* msg)
{
    /* Head */
    fputs(HEAD("Login"), stdout);

    /* Body */
    fputs(
        TAG("body", 0, 1, 1, 0,
            TAG("section", 1, 2, 1, 1, "class", "container",
                TAG("div", 1, 2, 1, 2, "class", "login",
                    H1("Login", 3),
                    TAG("form", 2, 3, 1, 3, "method", "post", "action", MAIN,
                        P(TAG("input", 4, 0, 0, 5, "type", "text", "name", "username", "value", "", "placeholder", "Username"), 4),
                        P(TAG("input", 4, 0, 0, 5, "type", "password", "name", "password", "value", "", "placeholder", "Password"), 4),
                        TAG("p", 1, 1, 1, 4, "class", "submit",
                            TAG("input", 3, 0, 0, 5, "type", "submit", "name", "commit", "value", "Login")
                        )
                    )
                ),
                msg == NULL ? "" :
                TAG("div", 1, 1, 1, 2, "class", "login-help",
                    TAG("p", 0, 1, 1, 3, msg)
                )
            )
        ),
    stdout);
}

void WELCOME(char* name)
{
    char** welcome;

    welcome = malloc(sizeof(char*));
    *welcome = malloc(10 + strlen(name));
    sprintf(*welcome, "Welcome %s!", name);

    /* Head */
    fputs(HEAD("Welcome"), stdout);

    /* Body */
    fputs(
        TAG("body", 0, 1, 1, 0,
            TAG("section", 1, 1, 1, 1, "class", "container",
                TAG("div", 1, 2, 1, 2, "class", "login",
                    H1(*welcome, 3),
                    TAG("form", 2, 1, 1, 3, "method", "post", "action", MAIN,
                        TAG("p", 1, 1, 1, 4, "class", "submit",
                            TAG("input", 3, 0, 0, 5, "type", "submit", "name", "commit", "value", "Logout")
                        )
                    )
                )
            )
        ),
    stdout);
}

/* Database operations */

char* password_of(char* user)
{
    int len;
    int id;
    char name[32];
    char username[32];
    char password[32];
    char line[128];
    char* res;
    char* db_filename;
    FILE *file;

    db_filename = "db";
    file = fopen(db_filename, "r");

    if (!file)
        return NULL;

    res = NULL;
    fgets(line, sizeof(line), file); // Skip the first line
    while (fgets(line, sizeof(line), file) != NULL)
    {
        sscanf(line, "%d\t%s\t%s\t%s\n", &id, name, username, password);
        if (!strncmp(username, user, 32))
        {
            len = strlen(password);
            res = malloc(len);
            memcpy(res, password, len);
            break;
        }
    }

    fclose(file);

    return res != NULL ? res : "";
}

char* name_of(char* user)
{
    int len;
    int id;
    char name[32];
    char username[32];
    char password[32];
    char line[128];
    char* res;
    char* db_filename;
    FILE *file;

    db_filename = "db";
    file = fopen(db_filename, "r");

    if (!file)
        return NULL;

    fgets(line, sizeof(line), file); // Skip the first line
    while (fgets(line, sizeof(line), file) != NULL)
    {
        sscanf(line, "%d\t%s\t%s\t%s\n", &id, name, username, password);
        if (!strncmp(username, user, 32))
        {
            len = strlen(name);
            res = malloc(len);
            memcpy(res, name, len);
            break;
        }
    }

    fclose(file);

    return res != NULL ? res : "";
}

/* POST data operations */

int read_login_data(char* data, char** username, char** password)
{
    int c;
    char u[32];
    char p[32];
    char cl[32];

    *username = malloc(32);
    *password = malloc(32);
    c = sscanf(data, "%[^=]=%[^&]&%[^=]=%[^&]&%s", u, *username, p, *password, cl);

    return c == 5 &&
        !strncmp(u, "username", 32) &&
        !strncmp(p, "password", 32) &&
        !strncmp(cl, "commit=Login", 32);
}

/* States logic */

int main()
{
    char* cookie;
    char* clenstr;
    char* data;
    char* username;
    char* password;
    int clen;

    cookie = getenv(COOKIE);
    if (cookie != NULL && strlen(cookie) > 0)
    {
        clenstr = getenv(CLEN);
        if (clenstr != NULL && sscanf(clenstr, "%d", &clen) == 1)
        {
            /* Logout POST */
            data = malloc(++clen);
            fgets(data, clen, stdin);

            /* Invalidate cookie */
            SETCOOKIE(cookie, 0); // Actually username not needed...

            /* Reload page */
            LOCATION(MAIN);
        }
        else if (!strncmp(cookie, INVALID_LOGIN, 32))
        {
                SETCOOKIE(INVALID_LOGIN, 0);
                CTYPE(HTML);
                HTML5();
                LOGIN("Wrong username or password.");
        }
        else
        {
            /* Welcome page */
            CTYPE(HTML);
            HTML5();
            WELCOME(name_of(cookie));
        }
    }
    else
    {
        clenstr = getenv(CLEN);
        if (clenstr != NULL && sscanf(clenstr, "%d", &clen) == 1)
        {
            /* Login POST */
            data = malloc(++clen);
            fgets(data, clen, stdin);

            /* Issue cookie after validating username and password */
            SETCOOKIE(read_login_data(data, &username, &password) &&
                !strncmp(password_of(username), password, 32) ?
                username : INVALID_LOGIN, 1);

            /* Reload page */
            LOCATION(MAIN);
        }
        else
        {
            /* Login form */
            CTYPE(HTML);
            HTML5();
            LOGIN(NULL);
        }
    }

    return 0;
}
