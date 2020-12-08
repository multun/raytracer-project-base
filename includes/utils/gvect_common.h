#pragma once

#define GVECT_U_CONCAT_(A, B) A##_##B
#define GVECT_U_CONCAT(A, B) GVECT_U_CONCAT_(A, B)
#define GVECT_FNAME(Suffix) GVECT_U_CONCAT(GVECT_NAME, Suffix)
