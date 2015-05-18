// c_inherit_and_polym.c
//
// Inheritance and polymorphism implemented in pure ANSI C, without support for
// access control and implicit `this`
//
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Header
/// types
typedef struct _animal animal;
typedef const char* (*fn_animal_name)(animal*);
typedef int (*fn_animal_legs)(animal*);
typedef void (*fn_animal_greet)(animal*, const char* who); // virtual
typedef void (*fn_free_animal)(animal*); // virtual

typedef struct _bird bird;
typedef void (*fn_bird_greet)(animal*, const char* who); // virtual
typedef void (*fn_bird_singit)(bird*);
typedef void (*fn_free_bird)(animal*);  // virtual

/// decl.
animal* new_animal(const char* name, int legs);
void free_animal(animal*);

animal* new_bird(const char* name, int legs, const char* sing);
void free_bird(animal*);

/// animal
typedef struct _animal
{
    // ptr to derived class
    void* dp_;
    // member vars
    const char* name_;
    int legs_;
    // member func
    fn_animal_name name;
    fn_animal_legs legs;
    fn_animal_greet greet; // virtual
    fn_free_animal free; // virtual
} animal;

/// bird
typedef struct _bird
{
    // ptr to base class
    animal* bp_;
    // member vars
    const char* sing_;
    // member func
    fn_bird_greet greet; // virtual
    fn_bird_singit singit;
    fn_free_bird free; // virtual
} bird;

// Main
int main(int argc, char* argv[])
{
    animal* animal_p = new_animal("whale", 0);
    printf("animal: %s, %d\n",
            animal_p->name(animal_p),
            animal_p->legs(animal_p));
    animal_p->greet(animal_p, "lei");
    animal_p->free(animal_p);

    animal* bp = new_bird("sparrow", 2, "bii");
    bird* bird_p = (bird*) bp->dp_;
    printf("bird: %s, %d\n",
            bp->name(bp), bp->legs(bp));
    bp->greet(bp, "lei");
    bird_p->singit(bird_p);
    bp->free(bp);
}

// Impl.
const char* _animal_name(animal*);
int _animal_legs(animal*);
void _animal_greet(animal*, const char* who);

animal* new_animal(const char* name, int legs)
{
    animal* this = (animal*) malloc(sizeof(animal));
    assert(this);
    this->dp_ = this; // point to self for base class
    this->name_ = name;
    this->legs_ = legs;
    this->name = _animal_name;
    this->legs = _animal_legs;
    this->greet = _animal_greet;
    this->free = free_animal;
    return this;
}

void free_animal(animal* this)
{
    assert(this);
    free(this);
}

const char* _animal_name(animal* this)
{
    assert(this);
    return this->name_;
}

int _animal_legs(animal* this)
{
    assert(this);
    return this->legs_;
}

void _animal_greet(animal* this, const char* who)
{
    assert(this);
    assert(who);
    printf("hello, %s. greet from animal(%s,%d).\n",
            who,
            this->name_, this->legs_);
}

void _bird_greet(animal*, const char* who);
void _bird_singit(bird*);

animal* new_bird(const char* name, int legs, const char* sing)
{
    animal* bp = new_animal(name, legs);
    bird* this = (bird*) malloc(sizeof(bird));
    assert(this);
    this->bp_ = bp; // point to base class
    this->sing_ = sing;
    this->singit = _bird_singit;
    this->free = free_bird;
    // update base 'vtable'
    bp->dp_ = this; // point to derived class
    // update virtual function pointers
    bp->greet = _bird_greet;
    bp->free = free_bird;

    return bp;
}

void free_bird(animal* this)
{
    assert(this && this->dp_ && this->dp_ != this);
    free((bird*) this->dp_);
    free(this);
}

void _bird_greet(animal* this, const char* who)
{
    assert(this && this->dp_ && this->dp_ != this);
    assert(who);
    bird* dp = (bird*) this->dp_;
    printf("hello %s. greeting from bird(%s,%d,%s).\n",
            who,
            this->name_, this->legs_, dp->sing_);
}

void _bird_singit(bird* this)
{
    assert(this && this->bp_->dp_ == this);
    printf("bird %s sings in %s\n",
            this->bp_->name_,
            this->sing_);
}
