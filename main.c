/* Extract path outlines using potrace; based on potracelib_demo.c */

#include <stdio.h>
#include "potracelib.h"
#include "bitmap.h"
#include "bitmap_io.h"
#include "curve.h"
#include "decompose.h"

static const potrace_param_t param={
  0,                             /* turdsize */
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

static void draw(path_t *plist, const char* s1, const char* s2, const char* s3) {
  int i,n;
  potrace_path_t *p;
  for (p=plist; p!=NULL; p=p->next) {
    n=p->priv->len;
    printf("%%{\n");
    printf("%% n: %d orientation: %c area: %d\n",n,p->sign,p->area);
    const char* op=s1;
    for (i=0; i<n; i++) {
      int j= (p->sign=='+') ? i : n-1-i;
      printf("%ld %ld %s %% %d\n",p->priv->pt[j].x,p->priv->pt[j].y,op,j);
      op=s2;
    }
    printf("%% closepath\n");
    printf("%%}\n");
    /* at the end of a group of a positive path and its negative children */
    if (p->next==NULL || p->next->sign=='+') {
      printf("%s",s3);
    }
  }
}

int main(void) {
  FILE *input=stdin;
  const double threshold=0.5;
  potrace_bitmap_t *bm;
  path_t *plist=NULL;

  /* read the image */
  if (bm_read(input,threshold,&bm)!=0) {
    fprintf(stderr,"bm_read failed\n");
    return 1;
  }

  /* process the image */
  if (bm_to_pathlist(bm,&plist,&param,NULL)!=0) {
    fprintf(stderr,"bm_to_pathlist failed\n");
    return 1;
  }

  /* output EPS */
  printf("%%!PS-Adobe-2.0 EPSF-2.0\n");
  printf("%%%%BoundingBox: 0 0 %d %d\n",bm->w,bm->h);
  printf("/p { 0.30 0 360 arc fill } bind def\n");
  printf("%% 1 setlinejoin 1 setlinecap\n");

  /* fill curves */
  printf("\n");
  printf("%% fill curves\n");
  printf("0.9 0.9 0.9 setrgbcolor\n");
  draw(plist,"moveto","lineto","fill\n");

  /* stroke curves */
  printf("\n");
  printf("%% stroke curves\n");
  printf("1 0 0 setrgbcolor\n");
  printf("0.15 setlinewidth\n");
  draw(plist,"moveto","lineto","stroke\n");

  /* draw points */
  printf("\n");
  printf("%% points\n");
  printf("0 0 0 setrgbcolor\n");
  draw(plist,"p","p","");

  /* done */
  printf("\n");
  printf("%%%%EOF\n");

  pathlist_free(plist);
  bm_free(bm);

  return 0;
}
