#include <iostream>
#include <graphics.h>
#include <winbgim.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

const int WIDTH = 600;
const int PADDING = WIDTH/24;

struct patrat
{
    int x;
    int y;
}p1, patr[9];

void drawP(patrat p)
{
    setcolor(COLOR(0,0,1));
    rectangle(p.x,p.y,p.x+WIDTH/3,p.y+WIDTH/3);
    setfillstyle(SOLID_FILL,COLOR(237, 224, 145));
    floodfill(p.x+20,p.y+20,COLOR(0,0,1));
}

void drawX(patrat p)
{
    setcolor(COLOR(51, 13, 219));
    line(p.x+PADDING,p.y+PADDING,p.x+WIDTH/3-PADDING,p.y+WIDTH/3-PADDING);
    line(p.x+WIDTH/3-PADDING,p.y+PADDING,p.x+PADDING,p.y+WIDTH/3-PADDING);
}

void drawO(patrat p)
{
    setcolor(COLOR(242, 39, 39));
    ellipse(p.x+WIDTH/6,p.y+WIDTH/6,0,360,WIDTH/10,WIDTH/8);
}

int m[3][3];
//int aux[3][3];
int gameover;

int checkWinner(int m[3][3])
{
    for(int i = 0; i < 3; i++){
        if(m[i][0] == m[i][1] && m[i][1] == m[i][2] && m[i][0] != 0){
           return 1;
        }
        if(m[0][i] == m[1][i] && m[1][i] == m[2][i] && m[0][i] != 0){
           return 1;
        }
    }
    if(m[1][1] != 0){
        if(m[0][0] == m[1][1] && m[1][1] == m[2][2]){
           return 1;
        }
        if(m[0][2] == m[1][1] && m[1][1] == m[2][0]){
          return 1;
        }
    }

    int remiza = 1;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(m[i][j] == 0)
                remiza = 0;
    if(remiza)
        return 2;

    return 0;
}

int best_i;
int best_j;
int diff;

bool xturn;

int minimax(int m[3][3], int depth, bool maximizingPlayer)
{
    if(checkWinner(m) == 1){
        if(maximizingPlayer)
            return -10;
        else
            return 10;
    }
    if(checkWinner(m) == 2)
        return 0;
    if(depth == 0)
        return 0;

    if(maximizingPlayer){
        int maxEval = -100;
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++){
                if(m[i][j] == 0){
                    m[i][j] = 2;

                    int eval = minimax(m, depth-1, false);
                    maxEval = max(eval, maxEval);

                    m[i][j] = 0;

                }
            }
        return maxEval;
    } else {
        int minEval = 100;
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++){
                if(m[i][j] == 0){
                    m[i][j] = 1;

                    if(checkWinner(m) && diff == 2){
                        best_i = i;
                        best_j = j;
                    }

                    int eval = minimax(m, depth-1, true);
                    minEval = min(eval, minEval);

                    m[i][j] = 0;

                }
            }
        return minEval;
    }
}

void drawLine()
{
    setcolor(COLOR(0,0,0));
    setlinestyle(0,0,0);
    for(int i = 0; i < 3; i++){
        if(m[i][0] == m[i][1] && m[i][1] == m[i][2] && m[i][0] != 0)
            line(PADDING/2, i*WIDTH/3+ WIDTH/6, WIDTH - PADDING/2, i*WIDTH/3+ WIDTH/6);
        if(m[0][i] == m[1][i] && m[1][i] == m[2][i] && m[0][i] != 0)
            line(i*WIDTH/3 + WIDTH/6, PADDING/2, i*WIDTH/3 + WIDTH/6, WIDTH - PADDING/2);
    }
    if(m[0][0] == m[1][1] && m[1][1] == m[2][2] && m[0][0] != 0)
        line(PADDING/2, PADDING/2, WIDTH-PADDING/2, WIDTH-PADDING/2);

    if(m[0][2] == m[1][1] && m[1][1] == m[2][0] && m[0][2] != 0)
        line(WIDTH-PADDING/2, PADDING/2, PADDING/2, WIDTH-PADDING/2);
}

int main()
{
    int gamemode;
    int first = 1;
    cout << "CHOOSE GAME MODE:\n  1 - Player vs Player\n  2 - Player vs AI\n";
    cin >> gamemode;
    if(gamemode == 2){
        cout << "CHOOSE DIFFICULTY:\n  1 - EASY\n  2 - MEDIUM\n  3 - HARD\n";
        cin >> diff;
        cout << "WHO GOES FIRST?\n  1 - YOU\n  2 - AI\n";
        cin >> first;
    }

    initwindow(WIDTH+1, WIDTH+1);

    setlinestyle(0,0,7);
    int k = 0;
    for(int j = 0; j < WIDTH; j += WIDTH/3)
        for(int i = 0; i < WIDTH; i += WIDTH/3){
            p1.x = i;
            p1.y = j;
            patr[k] = p1;
            k++;
            drawP(p1);
        }

    if(first == 1)
        xturn = true;
    else
        xturn = false;

    gameover = 0;
    while(!gameover){
        //PLAYER 1
        if(xturn){
            int x, y;
            getmouseclick(WM_LBUTTONDOWN,x,y);
            for(int i = 0; i < 9; i++){
                int x_mat = i/3;
                int y_mat = i-3*(i/3);
                if(x > patr[i].x && x < patr[i].x+WIDTH/3 && y > patr[i].y && y < patr[i].y+WIDTH/3 && m[x_mat][y_mat] == 0){
                    drawX(patr[i]);
                    m[x_mat][y_mat] = 1;
                    if(checkWinner(m) == 1){
                        cout << "X";
                        gameover = 1;
                        drawLine();
                    }
                    xturn = false;
                }
            }
        } else {
            //PLAYER 2
            if(gamemode == 1){
                int x, y;
                getmouseclick(WM_LBUTTONDOWN,x,y);
                for(int i = 0; i < 9; i++){
                    int x_mat = i/3;
                    int y_mat = i-3*(i/3);
                    if(x > patr[i].x && x < patr[i].x+WIDTH/3 && y > patr[i].y && y < patr[i].y+WIDTH/3 && m[x_mat][y_mat] == 0){
                        drawO(patr[i]);
                        m[x_mat][y_mat] = 2;
                        if(checkWinner(m) == 1){
                            cout << "O";
                            gameover = 1;
                            drawLine();
                        }
                        xturn = true;
                    }
                }
            } else if(gamemode == 2){
                //AI
                if(diff == 1){
                    srand (time(NULL));
                    int k = 1;
                    while(k){
                        int p = rand()%9;
                       // cout << p << "\n";
                        int i = p/3;
                        int j = p-3*(p/3);
                        if(m[i][j] == 0){
                            m[i][j] = 2;
                            drawO(patr[p]);
                            k = 0;
                        }
                    }
                    xturn = true;

                } else {
                    int depth;
                    if(diff == 2)
                        depth = 1;
                    else if(diff == 3)
                        depth = 1000;

                    int maxEval = -10;
                    for(int i = 0; i < 3; i++){
                        for(int j = 0; j < 3; j++){
                            if(m[i][j] == 0){
                                m[i][j] = 2;

                                int eval = minimax(m, depth, false);

                                if(eval > maxEval){
                                    maxEval = eval;
                                    best_i = i;
                                    best_j = j;
                                }

                                m[i][j] = 0;
                            }
                        }
                    }

                   // cout << maxEval << "\n";
                   // cout << best_i << " " << best_j << "\n";
                    m[best_i][best_j] = 2;
                    drawO(patr[best_i*3 + best_j]);

                  /*  for(int i = 0; i < 3; i++){
                        for(int j = 0; j < 3; j++)
                            cout << m[i][j] << " ";
                        cout << "\n";
                    }
                    cout << "\n";  */

                    if(checkWinner(m) == 1){
                        cout << "O";
                        gameover = 1;
                        drawLine();
                    }

                    xturn = true;

                }
            }
        }

        if(checkWinner(m) == 2){
            gameover = 1;
            cout << "remiza";
        }
    }

    getch();
    closegraph();

    return 0;
}
