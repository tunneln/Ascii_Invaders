/** 
Name: Noel Negusse
Eid: ntn443
*/

#include <ncurses.h>    // the ncurses library
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <unistd.h> 	// sleep
#include <list>
#include <iostream>

#include "timehelper.cpp"
#include "BigBaddie.h"
#include "SmallBaddie.h"
#include "PlayerShip.h"
#include "Projectile.h"
#include "Block.h"

using std::cout;
using std::endl;
using std::list;

int main(int argc, char const *argv[])
{
	srand (time(NULL)); /// seed to some random number based on time
	if ( initscr() == NULL ) { /// init the screen, defaults to stdscr
        	fprintf(stderr, "Error initialising ncurses.\n");
        	exit(EXIT_FAILURE);
    	}

    	noecho(); 		/// don't echo keypresses
    	keypad(stdscr, TRUE); 	/// set the keypad to stdscr
    	nodelay(stdscr, false);
    	curs_set(0); 		/// set cursor to invisible (if allowed by the os)
    	const int delay = 50; 	/// milliseconds
    	timeout(delay); 	/// set how long between keypresses (more or less)
	
	const static int xMAX = 20;	// X MAX FOR BOARD DIMENSION
       	const static int yMAX = 35; 	// Y MAX FOR BOARD DIMENSION

	bool quit = false;
	int ch = 0; int wee;
	int points = 0;
    	int elapsedTime = getElapsedTime();
  	int static ticks = 0;
	int rows = 2; int cols = 11;
	bool flag; int test = true; int hold; int blah = 0;
	char boarder[] = "#";

	PlayerShip player(xMAX-cols+2, yMAX+2);
	EnemyShip enemy;
	list<Projectile> playerP;
	list<Projectile> baddiesP;
	list<EnemyShip> enemies;
	list<Block> blocks;
	///ITERATORS
	list<EnemyShip>::iterator enemiesIter;
	list<Projectile>::iterator playerPIter;
	list<Projectile>::iterator baddiesPIter;
	list<Block>::iterator blocksIter;

	//MAKE/SET ENEMIES
	int middleinc = 7; bool space = true;
	for(int r = 2; r<(rows+2); r++){
		for(int c = middleinc+1; c<(cols+middleinc+1)*2; c++){
			if( (r+c)%2==0 ){
				if(space){
					enemy = BigBaddie(c-r,r);		//POLYMORPHISM!!!!!!
					space = false;
				}
				else{
					enemy = SmallBaddie(c-r,r); 	//PULYMORPHISM!!!!!!
					space = true;
				}
			}
			enemies.push_back(enemy);
		}
	}

	//MAKE/SET BLOCKS
	for(int b = 3; b<xMAX*2+3; b++){
		if(!((b+2)%((xMAX*2)/5)) || !((b+1)%((xMAX*2)/5)) || !((b)%((xMAX*2)/5))||
				!((b-1)%((xMAX*2)/5)) || !((b-2)%((xMAX*2)/5)))
		{
			blocks.push_back(Block(b-1,32));
		}
		if(!((b+1)%((xMAX*2)/5)) || !((b)%((xMAX*2)/5))||
				!((b-1)%((xMAX*2)/5)) )
		{
			blocks.push_back(Block(b-1,31));
		}
		if(!((b)%((xMAX*2)/5)))		
		{
			blocks.push_back(Block(b-1,30));
		}
	}

	if(has_colors() == false)
	{
		endwin();
		cout << "Your terminal does not support color" << endl;
		exit(1);
	}
	else{
	start_color();

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//LOOP FOR GAME
    	while (!quit)
	{
        	ch = getch(); //waits for a keypress and takes it in
        	erase(); /// erase the screen (after getch())
        	if ( ch != ERR) { /// user has a keypress
        	    /// this is to delay until the next tick
        	    elapsedTime = getElapsedTime();
        	    if (elapsedTime < delay){
        	        nsleep(delay-elapsedTime);
		    }
        	}
		ticks++;
		
////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//SET PLAYER POSITION AND ADD PLAYER PROJECTILES

       /*         if(player.getY()!=1 && ch==KEY_UP){ 		DON'T ALLOW UP AND DOWN MOVEMENT  */
			//player.decY();
		/*}*/

       /*         if(player.getY()<=yMAX && ch==KEY_DOWN){*/
			//player.incY();
		/*}*/

	
		if(player.getX()<=xMAX*2+3 && ch==KEY_RIGHT){
	        	player.incX(); 
		}
		
		if(player.getX()!=1 && ch==KEY_LEFT){
	        	player.decX();
	        }
		
		if(ch==' ' && playerP.size() <= 5){ 
			playerP.push_back(Projectile(player.getX(), player.getY()-1, '|'));
		}
		
		//CHECKS IF GAME WAS QUIT
		if(ch=='q') quit = true;
	
		//ADD/MAKE PROJECTILES FOR BADDIES
		enemiesIter = enemies.begin(); for(; enemiesIter!=enemies.end(); enemiesIter++) {
			if ((rand() % 10000) < (100*enemiesIter->getRate())){
				baddiesP.push_back(Projectile(enemiesIter->getX(), 
					enemies.front().getY()+2, enemiesIter->getGun()));
			}
		}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//CHECK FOR COLLISIONS - BETWEEN
		
		//'|' and 'W'/'u'	A.
		//'^'and '!'/'*' 	B.
		//'W'/'u' and '^'	C.
		//#/=/- and '!'/'*'	D.
		
		//A. 
		for(playerPIter = playerP.begin(); playerPIter != playerP.end(); playerPIter++){
			for(enemiesIter = enemies.begin(); enemiesIter != enemies.end(); enemiesIter++){
				if(playerPIter->isCollision(*enemiesIter)){
					if(enemiesIter->getShape() == 'u'){points+=2;}
					if(enemiesIter->getShape() == 'W'){points+=4;}
					playerPIter = playerP.erase(playerPIter);
					enemiesIter = enemies.erase(enemiesIter);
				}
			}
		}
		
		//B.
		flag = false;
		baddiesPIter = baddiesP.begin();
		for(; baddiesPIter != baddiesP.end(); baddiesPIter++){
			if(player.isCollision(*baddiesPIter))
			{
				baddiesPIter = baddiesP.erase(baddiesPIter);
				player.setShape(0);
				quit = true;
				flag = true;
				break;
			}
			if(flag==true) break;
		}

		//C.
		enemiesIter = enemies.begin();
		for(; enemiesIter != enemies.end(); enemiesIter++){
			if(player.isCollision(*enemiesIter))
			{
				quit = true;
				enemiesIter = enemies.erase(enemiesIter);
				break;
			}
		}	

		//D.
		baddiesPIter = baddiesP.begin(); blocksIter = blocks.begin();
		for(; baddiesPIter != baddiesP.end(); baddiesPIter++){
			for(; blocksIter != blocks.end(); blocksIter++){
				if(baddiesPIter->isCollision(*blocksIter)){
					baddiesPIter = baddiesP.erase(baddiesPIter);
				       /* if(blocksIter->getShape() == 0 || (blocksIter->getShape() != '=' */
					/*&& blocksIter->getShape() != '#' && blocksIter->getShape() != '-'))*/ 
						blocksIter = blocks.erase(blocksIter);
				       /* else*/
						/*blocksIter->timeStep(ticks);*/
				}
			}	
		}

////////////////////////////////////////////////////////////////////////////////////////////////////////////	
			//+++++++++++++++++++++++++PRINT BOARD+++++++++++++++++++++++++	

		//PRINT GAME DETAILS
		init_pair(1, COLOR_CYAN, COLOR_BLACK);
		attron(COLOR_PAIR(1));
		for(int x = 0; x<=23*2; x++){ //top and bottom grid
			mvprintw(0, x, boarder);
			mvprintw(38, x, boarder);
		}
		
		for(int y = 0; y<=38; y++){ //left and right grid
			mvprintw(y, 0, boarder);
			mvprintw(y, 23*2, boarder);
		}
		attroff(COLOR_PAIR(1));
		
		
		init_pair(2, COLOR_GREEN, COLOR_BLACK);		//COLOR GREEN
		attron(COLOR_PAIR(2));
		mvprintw(1, 1, "SCORE: %i", points);
	        mvaddch(player.getY(), player.getX(), player.getShape()); 		

		//PRINT BLOCKS
		blocksIter = blocks.begin();
		for(; blocksIter != blocks.end(); blocksIter++){
			mvaddch(blocksIter->getY(), blocksIter->getX(), blocksIter->getShape());
		}
		
		attroff(COLOR_PAIR(2));				// END GREEN COLOR
		
		mvprintw(40,0, "PRESS 'q' TO QUIT OUT OF GAME");

		//PRINT ENEMIES
	      	enemiesIter = enemies.begin();
		for(; enemiesIter != enemies.end(); enemiesIter++){
			if(rand()%10 < 5){
				init_pair(3, COLOR_BLUE, COLOR_BLACK);		//COLOR BLUE
				attron(COLOR_PAIR(3));
			}
			else{
				init_pair(3, COLOR_MAGENTA, COLOR_BLACK);		//COLOR MAGENTA
				attron(COLOR_PAIR(3));
			}
			mvaddch(enemiesIter->getY(), enemiesIter->getX(), enemiesIter->getShape());
			attroff(COLOR_PAIR(3));
		}

		//PRINT PROJECTILES
		baddiesPIter = baddiesP.begin();
		for(; baddiesPIter != baddiesP.end(); baddiesPIter++){
			mvaddch(baddiesPIter->getY(), baddiesPIter->getX(), baddiesPIter->getShape());
		}
	
		playerPIter = playerP.begin();
		for(; playerPIter != playerP.end(); playerPIter++){
			mvaddch(playerPIter->getY(), playerPIter->getX(), playerPIter->getShape());
		}
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//++++++++++MOVE ALL ENEMIES, PROJECTILES, AND *BLOCKS++++++++++

		//MOVE ALL ENEMIES
		if((enemies.front().getX() == 1)){
			test = true; 
		} 		
		if((enemies.back().getX() == xMAX*2+5)){
			test = false;
		}

	      	enemiesIter = enemies.begin(); hold = ticks;
		for(; enemiesIter != enemies.end(); enemiesIter++){	
			if(enemies.back().getY() == yMAX){
				quit = true;
				break;
			}
			if(blah==0 && !hold%480){
				enemiesIter->timeStep(2); //move down	
			}
			else{	
				if(!hold%540){
					if(++enemiesIter==enemies.end() && blah==0){
						blah++;
						--enemiesIter;
						enemiesIter->timeStep(0); //move right
						ticks = 0; // reset timer to mantian proper values
						break;
					}
					--enemiesIter;
					if(blah == 0){
						enemiesIter->timeStep(0); //move right
					}
					else
						enemiesIter->timeStep(2); //move down
				}
				else{
					if(hold%20==0){
						if(test)
							enemiesIter->timeStep(0); //move right
						else
							enemiesIter->timeStep(1); //move left
					}
				}
			}
		}
	
		//MOVE ALL PROJECTILES
		
		baddiesPIter = baddiesP.begin();
		for(; baddiesPIter != baddiesP.end(); baddiesPIter++){
			if((baddiesPIter->getY() <= yMAX+1)){
				baddiesPIter->timeStep(hold);
			}
			else{
				baddiesPIter = baddiesP.erase(baddiesPIter);
			}
		}
	
		playerPIter = playerP.begin();
		for(; playerPIter != playerP.end(); playerPIter++){
			if(playerPIter->getY() != 2)
				playerPIter->timeStep(hold);
			else
				playerPIter = playerP.erase(playerPIter);

		}
		

////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//CHECK FOR COLLISIONS - BETWEEN
		
		//'|' and 'W'/'u'	A.
		//'^'and '!'/'*' 	B.
		//'W'/'u' and '^'	C.
		//#/=/- and '!'/'*'	D.
		
		//A. 
		for(playerPIter = playerP.begin(); playerPIter != playerP.end(); playerPIter++){
			for(enemiesIter = enemies.begin(); enemiesIter != enemies.end(); enemiesIter++){
				if(playerPIter->isCollision(*enemiesIter)){
					if(enemiesIter->getShape() == 'u'){points+=2;}
					if(enemiesIter->getShape() == 'W'){points+=4;}
					playerPIter = playerP.erase(playerPIter);
					enemiesIter = enemies.erase(enemiesIter);
				}
			}
		}
		
		//B.
		flag = false;
		baddiesPIter = baddiesP.begin();
		for(; baddiesPIter != baddiesP.end(); baddiesPIter++){
			if(player.isCollision(*baddiesPIter))
			{
				baddiesPIter = baddiesP.erase(baddiesPIter);
				player.setShape(0);
				quit = true;
				flag = true;
				break;
			}
			if(flag==true) break;
		}

		//C.
		enemiesIter = enemies.begin();
		for(; enemiesIter != enemies.end(); enemiesIter++){
			if(player.isCollision(*enemiesIter))
			{
				quit = true;
				enemiesIter = enemies.erase(enemiesIter);
				break;
			}
		}	

		//D.
		baddiesPIter = baddiesP.begin(); blocksIter = blocks.begin();
		for(; baddiesPIter != baddiesP.end(); baddiesPIter++){
			for(; blocksIter != blocks.end(); blocksIter++){
				if(baddiesPIter->isCollision(*blocksIter)){
					baddiesPIter = baddiesP.erase(baddiesPIter);
				       /* if(blocksIter->getShape() == 0 || (blocksIter->getShape() != '=' */
					/*&& blocksIter->getShape() != '#' && blocksIter->getShape() != '-'))*/ 
						blocksIter = blocks.erase(blocksIter);
				       /* else*/
						/*blocksIter->timeStep(ticks);*/
				}
			}	
		}
////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		refresh(); // refresh the screen after adding everything
		move(0,0); /// move cursor to 0,0 (looks prettier if os doesn't allow invisible cursors)

		if(quit == true && enemies.empty()){
while(quit==true){
	wee++;
        		ch = getch(); //waits for a keypress and takes it in
        		erase(); /// erase the screen (after getch())
        		if ( ch != ERR) { /// user has a keypress
        		    /// this is to delay until the next tick
        		    elapsedTime = getElapsedTime();
        		    if (elapsedTime < 50){
        		        nsleep(50-elapsedTime);
			    }
        		}
				init_pair(4, COLOR_GREEN, COLOR_BLACK);		//COLOR GREEN
				attron(COLOR_PAIR(4));
mvaddstr(0,0,"	        __     ______  _    _        ");
mvaddstr(1,0,"         \\ \\   / / __ \\| |  | |       ");
mvaddstr(2,0,"          \\ \\_/ / |  | | |  | |       ");
mvaddstr(3,0,"           \\   /| |  | | |  | |       "); 
mvaddstr(4,0,"            | | | |__| | |__| |       "); 
mvaddstr(5,0,"         __ |_|  \\____/_\\____/   _ _  "); 
mvaddstr(6,0,"         \\ \\        / /_   _| \\ | | | "); 
mvaddstr(7,0,"          \\ \\  /\\  / /  | | |  \\| | | "); 
mvaddstr(8,0,"           \\ \\/  \\/ /   | | | . ` | | "); 
mvaddstr(9,0,"            \\  /\\  /   _| |_| |\\  |_| "); 
mvaddstr(10,0,"             \\/  \\/   |_____|_| \\_(_) ");
mvaddstr(11,0,"\n'q' to quit");
				attroff(COLOR_PAIR(4));
}break;
}	
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}

	if(quit == true && !enemies.empty()){ 
		while(quit == true){
			wee++;
        		ch = getch(); //waits for a keypress and takes it in
        		erase(); /// erase the screen (after getch())
        		if ( ch != ERR) { /// user has a keypress
        		    /// this is to delay until the next tick
        		    elapsedTime = getElapsedTime();
        		    if (elapsedTime < 50){
        		        nsleep(50-elapsedTime);
			    }
        		}
				init_pair(5, COLOR_RED, COLOR_BLACK);		//COLOR RED
				attron(COLOR_PAIR(5));
mvaddstr(0,0,"        __     ______  _    _           ");
mvaddstr(1,0,"        \\ \\   / / __ \\| |  | |          ");
mvaddstr(2,0,"         \\ \\_/ / |  | | |  | |          ");
mvaddstr(3,0,"          \\   /| |  | | |  | |          ");
mvaddstr(4,0,"           | | | |__| | |__| |          ");
mvaddstr(5,0,"         _ |_|  \\____/ \\____/_______ _  ");
mvaddstr(6,0,"        | |    / __ \\ / ____|__   __| | ");
mvaddstr(7,0,"        | |   | |  | | (___    | |  | | ");
mvaddstr(8,0,"        | |   | |  | | (___    | |  | | ");
mvaddstr(9,0,"        | |   | |  | |\\___ \\   | |  | | ");
mvaddstr(10,0,"        | |___| |__| |____) |  | |  |_| ");
mvaddstr(11,0,"        |______\\____/|_____/   |_|  (_) ");
mvaddstr(12,0,"\n'q' to quit");
				attroff(COLOR_PAIR(5));
if(ch == 'q') quit = false;			
		}		
	}
	if(has_colors() == true)
		endwin();  /// cleanup the window


	return 0;
}
