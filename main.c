/* Extract path outlines using potrace; based on potracelib_demo.c */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "potracelib.h"
#include "bitmap.h"
#include "bitmap_io.h"
#include "decompose.h"

/* default parameters */
static const potrace_param_t param_default = {
  2,                             /* turdsize */
  POTRACE_TURNPOLICY_MINORITY,   /* turnpolicy */
  1.0,                           /* alphamax */
  1,                             /* opticurve */
  0.2,                           /* opttolerance */
  {
    NULL,                        /* callback function */
    NULL,                        /* callback data */
    0.0, 1.0,                    /* progress range */
    0.0,                         /* granularity */
  },
};

int main(void) {
  int i, n;
  potrace_bitmap_t *bm;
  potrace_param_t param;
  potrace_path_t *p;
  path_t *plist = NULL;

  /* read the image */
  if (bm_read(stdin, 0.5, &bm)!=0) {
    fprintf(stderr, "bm_read failed\n");
    return 1;
  }

  /* set tracing parameters, starting from defaults */
  param = param_default;
  param.turdsize = 0;

  /* process the image */
  if (bm_to_pathlist(bm, &plist, &param, NULL)!=0) {
    fprintf(stderr, "bm_to_pathlist failed\n");
    return 1;
  }

  /* output vector data as EPS file */
  printf("%%!PS-Adobe-2.0 EPSF-2.0\n");
  printf("%%%%BoundingBox: 0 0 %d %d\n", bm->w, bm->h);
  printf("/p { 0.30 0 360 arc fill } bind def\n");

  /* draw each curve */
  printf("\n");
  printf("%% curves\n");
  printf("1 0 0 setrgbcolor\n");
  printf("0.15 setlinewidth\n");
  for (p = plist; p != NULL; p = p->next) {
    n = p->priv->len;
    printf("%% n=%d orientation=%c area=%d\n", n,p->sign,p->area);
    const char* op="moveto";
    for (i=0; i<n; i++) {
	printf("%ld %ld %s %% %d\n", p->priv->pt[i].x, p->priv->pt[i].y,op,i);
	op="lineto";
    }
    /* at the end of a group of a positive path and its negative children, fill. */
    if (p->next == NULL || p->next->sign == '+') {
      printf("stroke %% fill\n");
    }
  }

  /* draw points */
  printf("\n");
  printf("%% points\n");
  printf("0 0 0 setrgbcolor\n");
  for (p = plist; p != NULL; p = p->next) {
    n = p->priv->len;
    const char* op="p";
    for (i=0; i<n; i++) {
	printf("%ld %ld %s %% %d\n", p->priv->pt[i].x, p->priv->pt[i].y,op,i);
    }
  }
  printf("\n");
  printf("%%EOF\n");

  pathlist_free(plist);
  bm_free(bm);

  return 0;
}
