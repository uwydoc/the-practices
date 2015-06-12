// unnamed_struct_member.c
//
#include <stdio.h>
#include <time.h>

typedef struct {
    union {
        char* school;
        char* company;
    };
    union {
        char* major;
        char* title;
    };
    time_t started;
    time_t left;
    char* description;
} resume_item_t;

void resume_print_education_item(const resume_item_t* item);
void resume_print_profession_item(const resume_item_t* item);

int main(int argc, char* argv[])
{
    resume_item_t zju = {
        .school = "Zhejiang University",
        .major = "Computer Science and Technology",
        .started = 1220198400,
        .left = 1340208000
    };
    resume_item_t vobile = {
        .company = "Vobile, Inc.",
        .title = "Software Engineer (C++)",
        .started = 1341763200,
        .left = 1373385600
    };
    resume_item_t sdri = {
        .company = "SSGG Data Science Research Institute, Inc.",
        .title = "Senior Software Engineer",
        .started = 1416672000,
        .left = 0
    };

    resume_print_education_item(&zju);
    resume_print_profession_item(&vobile);
    resume_print_profession_item(&sdri);

    return 0;
}

///
void resume_print_education_item(const resume_item_t* item)
{
    if (!item)
        return;

    static int count = 1;
    if (count == 1)
        printf("Education\n-----\n");
    char started[16] = "NOW";
    if (item->started > 0)
        strftime(started, 15, "%b %d %Y", localtime(&item->started));
    char left[16] = "NOW";
    if (item->left > 0)
        strftime(left, 15, "%b %d %Y", localtime(&item->left));
    printf("*  School: %s\n"
           "    Major: %s\n"
           "  Started: %s\n"
           "     Left: %s\n"
           , item->school
           , item->major
           , started
           , left);
    count++;
}

void resume_print_profession_item(const resume_item_t* item)
{
    if (!item)
        return;

    static int count = 1;
    if (count == 1)
        printf("Profession\n-----\n");
    char started[16] = "NOW";
    if (item->started > 0)
        strftime(started, 15, "%b %d %Y", localtime(&item->started));
    char left[16] = "NOW";
    if (item->left > 0)
        strftime(left, 15, "%b %d %Y", localtime(&item->left));
    printf("* Company: %s\n"
           "    Title: %s\n"
           "  Started: %s\n"
           "     Left: %s\n"
           , item->company
           , item->title
           , started
           , left);
    count++;
}
