#pragma once 

#include <typedef.h>
#include <obj/view.h>

typedef struct _ViewController_t ViewController_t;

typedef ViewController_t* (*UpdateView) (ViewController_t* self, uint8_t reload);

typedef struct _ViewController_t {
   View_t* _view;
   ViewController_t* _prevView;
   ViewController_t* _nextView;
   UpdateView Update;
} ViewController_t;
