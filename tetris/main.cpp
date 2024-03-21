#define WIDTH 13
#define HEIGHT 31
#include <iostream>
#include <ncurses.h>
#include <unistd.h>
using namespace std;
int well[HEIGHT+1][WIDTH+1];
int score=0;
int chr;
int c,d,de,m;
int a,b,j,k,z;
int maxx,maxy;
int mino[5][5];
int delay = 250000;

int alltet[7][5][5] = {
   0, 0, 0, 0, 0,  0, 0, 1, 0, 0,  0, 0, 1, 0, 0,  0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
   0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 1, 1, 0, 0,  0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0,  0, 0, 1, 0, 0,  0, 1, 1, 0, 0,  0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0,  0, 1, 0, 0, 0,  0, 1, 1, 0, 0,  0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0,  0, 0, 1, 0, 0,  0, 1, 1, 1, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0,  0, 0, 1, 1, 0,  0, 0, 1, 0, 0,  0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0,  0, 1, 1, 0, 0,  0, 0, 1, 0, 0,  0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
 };

int x[1000];

void init_well()
{
  int i,j;
  for (i=1;i<=HEIGHT;++i) for (j=1;j<=WIDTH;++j) well[i][j]=0;
}
void print_well()
{
  int i,j;
  for (i=HEIGHT;i>0;--i)
    {
      for (j=WIDTH;j>0;--j) if (well[i][j]==0) cout<<" . ";
	else cout<<" ! ";
      cout<<endl;
    }
  cout<<endl<<endl;
}
void init_ab()
{
  for (k=4;k>=1;--k) 
	{
	  for (z=4;z>=1;--z) if (mino[z][k]==1) break;
	  if (z!=0) break;
	}  /*k = ultima coloana negoala */
      for (b=1;b<=4;++b) 
	  {
	    for (z=1;z<=4;++z) if (mino[z][b]==1) break;
	    if (z!=5) break;
	  } /*b = prima coloana negoala */
      for (j=4;j>=1;--j) 
	{
	  for (z=4;z>=1;--z) if (mino[j][z]==1) break;
	  if (z!=0) break;
	}  /*j = ultima linie negoala */
      for (a=1;a<=4;++a) 
	  {
	    for (z=1;z<=4;++z) if (mino[a][z]==1) break;
	    if (z!=5) break;
	  } /*a = prima linie negoala */
}
int rx = 100; 
int ra = 117;
int rb = 83; // generator congruential liniar, x' = (a * x + b) mod n
int random() {
  rx = (rx * ra + rb) % 6999;
  // mvprintw(9, 20, "%04d", rx);
  //  fprintf(stderr, "=== %d ===\n", rx/1000);
  return rx / 1000;
}

void gen_mino()
{
  int i,j;
  int x;
  x = random();
  for (i=1;i<=4;++i) for (j=1;j<=4;++j) mino[i][j]=alltet[x][i][j];
  //    for (i=1;i<=4;++i) for (j=1;j<=4;++j) mino[i][j]=0;
  //  mino[2][3]=mino[3][3]=mino[4][3]=mino[4][4]=1;
}
void rot_left()
{
  int minc[5][5],i,j;
  for (i=1;i<=4;++i) for (j=1;j<=4;++j) minc[5-i][j]=mino[j][i];
  for (i=1;i<=4;++i) for (j=1;j<=4;++j) mino[i][j]=minc[i][j];
}
void rot_right()
{
  int minc[5][5],i,j;
  for (i=1;i<=4;++i) for (j=4;j>=1;--j) minc[i][5-j]=mino[j][i];
  for (i=1;i<=4;++i) for (j=1;j<=4;++j) mino[i][j]=minc[i][j];
}
void clear_line(int f)
{
  int ik,jk;
  for(ik=f;ik>2;--ik) for (jk=1;jk<=WIDTH;++jk) well[ik][jk]=well[ik-1][jk];
  for (jk=1;jk<=WIDTH;++jk) well[2][jk]=0;
  score+=20;
  mvprintw(4, 20, "                 ");
  mvprintw(4, 20, "score = %d", score);
  refresh();
}
void show_mino()
{
  int i,j;
  for (i=1;i<=4;++i) for (j=1;j<=4;++j) 
  {
    if (mino[i][j]==1) {
      attron(COLOR_PAIR(1));
      mvprintw(i+20,j+20,"=");
      attroff(COLOR_PAIR(1));
    }
    else mvprintw(i+20,j+20," ");
  }
}
int main()
{
  fprintf(stderr, "random = %d\n", random());
  initscr();
  start_color();
  init_pair(1, COLOR_BLACK, COLOR_RED);
  noecho();
  nodelay(stdscr,TRUE);
  curs_set(FALSE);
  cbreak();
  getmaxyx(stdscr,maxx,maxy);
  //init_well();
  int i=2;
  int ja,ka,ik,jk;
  d=0;
  int nth=0;
  //print_well();
  keypad(stdscr,TRUE);
  clear();
  gen_mino();
  for (i=0;i<=HEIGHT;++i) mvprintw(i, WIDTH, "&");
  for (i=0;i<=HEIGHT;++i) mvprintw(i, 0, "&");
  //for (i=0;i<=WIDTH;++i) mvprintw(0, i, "&");
  for (i=0;i<=WIDTH;++i) mvprintw(HEIGHT, i, "&");
  i=1;
  while (1)
    {
      nth++;
      //mvprintw(2, 20, "%d", nth);
      mvprintw(4, 20, "                 ");
      mvprintw(4, 20, "score = %d", score);
      c=getch();
      de=d;
      init_ab();
      if (c=='f') delay -= delay/10;
      if (c=='s') delay += delay/10;
      if (c==261) ++d;
      else if (c==260) --d;
      if (d>=WIDTH/2-k+b+1) d=WIDTH/2-k+b;
      if (d<=-WIDTH/2) d=-WIDTH/2+1;
      //mvprintw(5, 20, "a = %d", a);
      //mvprintw(7, 20, "b = %d", b);
      //mvprintw(9, 20, "j = %d", j);
      //mvprintw(11, 20, "k = %d", k);
      //mvprintw(13, 20, "d = %d", d);
      ka=1;
      
      for (ja=0;ja<=k-b;++ja) 
	{
	  if (mino[j][ja+b]==1) if (well[i+j-a][WIDTH/2+ja+de]==1) ka=0;
	  if (mino[j][ja+b]==0) for (z=1;z<=j-a;++z) if (well[i+j-a-z][WIDTH/2+ja+de]==1 && mino[j-z][ja+b]==1) ka=0;
 	} 
      if (ka==1)
	{
	  /*well[i-1][WIDTH/2+de]=0;
	  mvprintw(i-1, WIDTH/2+de, " ");
	  mvprintw(i, WIDTH/2+d, "!");
	  well[i][WIDTH/2+d]=1;*/
	  ja=a;ka=b;
	  while (ja<5) /* sterge mino desenat anterior */
	    {
	      ka = b;
	      while (ka<5)
		{
		  if (mino[ja][ka]==1) 
		    {
		      mvprintw(i+ja-a-1,WIDTH/2+ka-b+de," ");
		      //well[i+ja-a-1][WIDTH/2+ka-b+de]=0;
		    }
		  ++ka;
		}
	      ++ja;
	    }
	  if (c==32) 
	    {
	      rot_right();
	    }
	  ka=1;
	  for (ja=0;ja<=j-a;++ja) if (well[i+ja][WIDTH/2+d]==1 && mino[ja+a][b]==1) ka=0;
	  if (ka==0) ++d;
	  ka=1;
	  for (ja=0;ja<=j-a;++ja) if (well[i+ja][WIDTH/2+d+k-b]==1 && mino[ja+a][k]==1) ka=0;
	  if (ka==0) --d;
	  ja=a; ka=b;
          while (ja<5)
	    {
	      ka=b;
	      while (ka<5)
		{
		  if (mino[ja][ka]==1) 
		    {
		      mvprintw(i+ja-a,WIDTH/2+ka-b+d,"!");
		      //well[i+ja-a][WIDTH/2+ka-b+d]=1;
		    }
		  ++ka;
		}
	      ++ja;
	    }
	  if (c==32) for (ik=1;ik<HEIGHT;++ik) for (jk=1;jk<WIDTH;++jk){ 
			       if (well[ik][jk]==1) mvprintw(ik, jk, "!");
			       else mvprintw(ik, jk, " ");
	      }
	  if (i+j-a>=HEIGHT-1) 
	  {
	    ja=a;
	    while (ja<5)
	    {
	      ka=b;
	      while (ka<5)
		{
		  if (mino[ja][ka]==1) 
		    {
		      //mvprintw(i+ja-a,WIDTH/2+ka-b+d,"!");
		      well[i+ja-a][WIDTH/2+ka-b+d]=1;
		    }
		  ++ka;
		}
	      ++ja;
	    }
	    gen_mino();
	    i=1;
	    d=0;
	    ++score;
	    ++score;
	    for (z=HEIGHT-1;z>1;--z) 
	      {
		ka=1;
		for (ja=1;ja<WIDTH;++ja) if (well[z][ja]==0) ka=0;
		if (ka==1) clear_line(z);
	      }
	    for (ik=1;ik<HEIGHT;++ik) for (jk=1;jk<WIDTH;++jk){ 
			       if (well[ik][jk]==1) mvprintw(ik, jk, "!");
			       else mvprintw(ik, jk, " ");
	      }

	  }
	  refresh();
	  //print_well();
	}
      else 
	{
	  if (i<3) 
	    {
	      //well[i-1][WIDTH/2+d]=1;
	      //mvprintw(i-1, WIDTH/2+d, "!");
	      //refresh();
	      //print_well();
	      break;
	    }
	  else 
	    {
	      ja=a;
	      while (ja<5) 
		{
		  ka=b;
		  while (ka<5)
		    {
		      if (mino[ja][ka]==1) 
			{
			  //mvprintw(i+ja-a,WIDTH/2+ka-b+d,"!");
			  well[i-1+ja-a][WIDTH/2+ka-b+de]=1;
			}
		      ++ka;
		    }
		  ++ja;
		}
	      gen_mino();
	      i=1;
	      d=0;
	      ++score;
	      ++score;
	      for (z=HEIGHT-1;z>1;--z) 
		{
		  ka=1;
		  for (ja=1;ja<WIDTH;++ja) if (well[z][ja]==0) ka=0;
		  if (ka==1) clear_line(z);
		}
	      for (ik=1;ik<HEIGHT;++ik) for (jk=1;jk<WIDTH;++jk){ 
			       if (well[ik][jk]==1) mvprintw(ik, jk, "!");
			       else mvprintw(ik, jk, " ");
		}

	    }
      	}
      show_mino();
      ++i;
      usleep(delay);
    }
  endwin();
}
