#pragma once

// typedef void (*UpdateViewData) (void);
// typedef void (*SelectView) (void);
// typedef void (*UnselectView) (void);

// typedef struct _ViewController_t
// {
//    SelectView Load;
//    UnselectView Unload;
//    UpdateViewData UpdateView;
// } ViewController_t;

typedef void (*DrawView) (void);

// abstract type
typedef struct _View_t {
   DrawView Draw;
} View_t;
