
#include <iostream>
#include <SDL.h>
#include<time.h>
#include <SDL_mixer.h>

//#define WAV_PATH "low.wav"
#define MUS_PATH "Welcome-to-Planet-Urf.wav"

// Our wave file
Mix_Chunk *wave = NULL;
// Our music file
Mix_Music *music = NULL;

//#include <SDL2/SDL.h>

using namespace std;

//**************************************************************
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "2048 in SDL";

void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();
//**************************************************************

// Hai hàm về texture, lấy nguyên về từtutorial tại:
// http://www.willusher.io/sdl2%20tutorials/2013/08/17/lesson-2-dont-put-everything-in-main

// Hàm nạp texture từ file ảnh, để vẽ lên renderer tương ứng
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);

// Hàm đưa texture ra renderer (để hiển thị ra màn hình) tại toạ độ (x,y) và giữ nguyên kích cỡ ban đầu của ảnh
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);

// Hàm (cùng trên nhưng khác tham số với hàm ngay trên)
// đưa texture ra renderer (để hiển thị ra màn hình) tại toạ độ (x,y) và
// với kích cỡ rộng (w) và cao (h) cho trước.
// Ảnh sẽ được co dãn (phóng to thu nhỏ) cho khớp kích cỡ này.
// Đây là hàm không có trong tutorial ban đầu, được sửa từ hàm trong tutorial.
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);
int bgrid[4][4];
int grid[4][4];
char getControl(){
    char c;
    cin >> c;
    return c;
}
void backupGrid(){
    for(int i = 0; i <4 ; i ++)
        for(int j = 0 ; j < 4 ; j ++)
            bgrid[i][j] =  grid[i][j];
}

// check block move
bool checkToSpawn(){
    for(int i = 0; i <4 ; i ++)
        for(int j = 0 ; j < 4 ; j ++)
            if(bgrid[i][j] !=  grid[i][j]) return 1;
    return 0;
}

int maxTile()
{
    int maxx = 2;
    for(int i = 0; i <4 ; i ++){
        for(int j = 0 ; j < 4 ; j ++){
            if( grid[i][j] > maxx ) maxx = grid[i][j];
        }
    }
    return maxx;

}

bool full(){
    for(int i = 0; i <4 ; i ++){
        for(int j = 0 ; j < 4 ; j ++){
            if( grid[i][j] == 0 ) return 0;
        }
    }
    return 1;
}

bool checkIfCanMove(){
    int checkgrid[6][6];
    for(int i = 0 ; i < 6 ; i ++){
        for(int j = 0 ; j < 6 ; j ++){
            checkgrid[i][j] = 0;
        }
    }
    for(int i = 1 ; i <=4 ; i ++){
        for(int j = 1 ; j <= 4 ; j ++){
            checkgrid[i][j] = grid[i-1][j-1];
        }
    }
    for(int i = 1 ; i <=4 ; i ++){
        for(int j = 1 ; j <= 4 ; j ++){
            if( checkgrid[i][j] != 0){
                if(checkgrid[i][j] == checkgrid[i-1][j] || checkgrid[i][j] == checkgrid[i+1][j] || checkgrid[i][j] == checkgrid[i][j-1] || checkgrid[i][j] == checkgrid[i][j+1] ) return 1;
            }
        }
    }

    return 0;

}
bool checkEndGame(){
    if( (full() == 1 && checkIfCanMove() == 0  ) || maxTile() == 2048 ) return 1;
    return 0;
}

void updateGrid( char direction)
{

    switch(direction)
	{
		case 'w':
					for(int i=0;i<4;i++)
						for(int j=0;j<3;j++)
						{
							if(grid[j][i]&&grid[j][i]==grid[j+1][i])
							{
								grid[j][i]+=grid[j+1][i];
								grid[j+1][i]=0;

							}
						}break;

		case 's':
					for(int i=0;i<4;i++)
						for(int j=3;j>0;j--)
						{
							if(grid[j][i]&&grid[j][i]==grid[j-1][i])
							{
								grid[j][i]+=grid[j-1][i];
								grid[j-1][i]=0;

							}
						}break;

		case 'a':
					for(int i=0;i<4;i++)
						for(int j=0;j<3;j++)
						{
							if(grid[i][j]&&grid[i][j]==grid[i][j+1])
							{
								grid[i][j]+=grid[i][j+1];
								grid[i][j+1]=0;

							}
						}break;

		case 'd':
					for(int i=0;i<4;i++)
						for(int j=3;j>0;j--)
						{
							if(grid[i][j]&&grid[i][j]==grid[i][j-1])
							{
								grid[i][j]+=grid[i][j-1];
								grid[i][j-1]=0;
                            }
						}break;


	}

}

void fill_move( char direction ){

    switch(direction){
    case 'w':
        for(int i=0;i<4;i++)
            for(int j=0;j<4;j++)
            {
                if(!grid[j][i])
                {
                    for(int k=j+1;k<4;k++)
                        if(grid[k][i])
                        {
                            grid[j][i]=grid[k][i];
                            grid[k][i]=0;
                            break;
                        }
                }

            }
        break;
    case 's':
					for(int i=0;i<4;i++)
						for(int j=3;j>=0;j--)
						{
							if(!grid[j][i])
							{
								for(int k=j-1;k>=0;k--)
									if(grid[k][i])
									{
										grid[j][i]=grid[k][i];
										grid[k][i]=0;
										break;
									}
							}

						}break;
		case 'a':
					for(int i=0;i<4;i++)
						for(int j=0;j<4;j++)
						{
							if(!grid[i][j])
							{
								for(int k=j+1;k<4;k++)
									if(grid[i][k])
									{
										grid[i][j]=grid[i][k];
										grid[i][k]=0;
										break;
									}
							}

						}break;


		case 'd':
					for(int i=0;i<4;i++)
						for(int j=3;j>=0;j--)
						{
							if(!grid[i][j])
							{
								for(int k=j-1;k>=0;k--)
									if(grid[i][k])
									{
										grid[i][j]=grid[i][k];
										grid[i][k]=0;
										break;
									}
							}

						}break;

    }

}

int random_index(int x)
{
    int index;
    index=rand()%x+0;

    return index;
}

void disp(int a[4][4])
{
	cout<<"\n\t\tPress Esc anytime to quit the game";
	cout<<"\n\n\n\n";
	int i,j;
	for(i=0;i<4;i++)
	{
		cout<<"\t\t\t\t-----------------\n\t\t\t\t";
		for(j=0;j<4;j++)
		{
			if(a[i][j]==0) cout<<"|   ";
			else
				cout<<"| "<<a[i][j]<<" ";
		}
		cout<<"|"<<endl;
	}
	cout<<"\t\t\t\t-----------------\n";
}
void spawn()
{
        // p : tinh xac xuat ra ngau nhien so 4 nhieu hon 2
		int i,j, p ;

		do
		{
		    i=random_index(4);
		    j=random_index(4);
		    p=random_index(10);

		}while(grid[i][j]);

		if(p<2)
		    grid[i][j]=4;
		else
		    grid[i][j]=2;
}

void startGrid(){

    int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			grid[i][j]=0;

	i=random_index(4);
	j=random_index(4);

		grid[i][j]=2;

	i=random_index(4);
	j=random_index(4);

		grid[i][j]=2;
}

int main(int argc, char* argv[] )
{
    srand(time(0));
    startGrid();
    //disp(grid);

    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);
    SDL_Event e;

    if (SDL_Init(SDL_INIT_AUDIO) < 0)
		return -1;

	//Initialize SDL_mixer
	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
		return -1;

	// Load our sound effect
	//wave = Mix_LoadWAV(WAV_PATH);
	//if (wave == NULL)
		//return -1;

	// Load our music
	music = Mix_LoadMUS(MUS_PATH);
	if (music == NULL)
		return -1;

	//if ( Mix_PlayChannel(-1, wave, 0) == -1 )
		//return -1;

	if ( Mix_PlayMusic( music, -1) == -1 )
		return -1;


    //Nạp ảnh nền vào một biến texture
    SDL_Texture *background = loadTexture("pinkbg.bmp", renderer);
    //Nạp ảnh vào một biến texture
    SDL_Texture *image2 = loadTexture("2crop.bmp", renderer);
    SDL_Texture *image4 = loadTexture("4crop.bmp", renderer);
    SDL_Texture *image8 = loadTexture("8crop.bmp", renderer);
    SDL_Texture *image16 = loadTexture("16crop.bmp", renderer);
    SDL_Texture *image32 = loadTexture("32crop.bmp", renderer);
    SDL_Texture *image64 = loadTexture("64crop.bmp", renderer);
    SDL_Texture *image128 = loadTexture("128crop.bmp", renderer);
    SDL_Texture *image256 = loadTexture("256crop.bmp", renderer);
    SDL_Texture *image512 = loadTexture("512crop.bmp", renderer);
    SDL_Texture *image1024 = loadTexture("1024crop.bmp", renderer);
    SDL_Texture *image2048 = loadTexture("2048crop.bmp", renderer);
    SDL_Texture *imageblank = loadTexture("blank.bmp", renderer);


    if (background == nullptr || image2 == nullptr || image4 == nullptr || image8 == nullptr|| image16 == nullptr|| image32 == nullptr|| image64 == nullptr|| image128 == nullptr|| image256 == nullptr || image512 == nullptr|| image1024 == nullptr|| image2048 == nullptr){
        SDL_DestroyTexture(background);
        SDL_DestroyTexture(image2);
        SDL_DestroyTexture(image4);
        SDL_DestroyTexture(image8);
        SDL_DestroyTexture(image16);
        SDL_DestroyTexture(image32);
        SDL_DestroyTexture(image64);
        SDL_DestroyTexture(image128);
        SDL_DestroyTexture(image256);
        SDL_DestroyTexture(image512);
        SDL_DestroyTexture(image1024);
        SDL_DestroyTexture(image2048);
        quitSDL(window, renderer);
    }

    //Xoá màn hình
    SDL_RenderClear(renderer);




    //Vẽ ảnh nền vào toàn bộ cửa sổ
    renderTexture(background, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white
    //SDL_RenderDrawPoint(renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red
    //SDL_RenderDrawLine(renderer, 100, 100, 200, 200);



    int cell_num = 4;
    int c_pad = 120 , r_pad = (SCREEN_WIDTH-SCREEN_HEIGHT)/2+ c_pad;

    // c_pad : cách trên
    // r_pad :  cách bên

    int cell_size = ( SCREEN_HEIGHT - 2*c_pad)/cell_num ;





    //SDL_RenderPresent(renderer);


    while( !checkEndGame() ){
        Mix_PlayingMusic();
        backupGrid();
        char direction = '0';
        // Nếu không có sự kiện gì thì tiếp tục trở về đầu vòng lặp
        if ( SDL_WaitEvent(&e) == 0) continue;

        // Nếu sự kiện là kết thúc (như đóng cửa sổ) thì thoát khỏi vòng lặp
        if (e.type == SDL_QUIT) break;

        // Nếu có một phím được nhấn, thì xét phím đó là gì để xử lý tiếp
        if (e.type == SDL_KEYDOWN) {
            // Nếu nhấn phìm ESC thì thoát khỏi vòng lặp
            if (e.key.keysym.sym == SDLK_ESCAPE) break;

            // Nếu phím mũi tên trái, dịch sang trái
            // (cộng chiều rộng, trừ bước, rồi lấy phần dư để giá trị luôn dương, và hiệu ứng quay vòng)
            if (e.key.keysym.sym == SDLK_LEFT) {
                    direction = 'a';
                    //filled_rect.x = (filled_rect.x + SCREEN_WIDTH - step) % SCREEN_WIDTH;
            }

            // Tương tự với dịch phải, xuống và lên
            if (e.key.keysym.sym == SDLK_RIGHT) {
                    direction = 'd';
                //filled_rect.x = (filled_rect.x + step) % SCREEN_WIDTH;
            }
            if (e.key.keysym.sym == SDLK_DOWN) {
                    direction = 's';
                    //filled_rect.y = (filled_rect.y + step) % SCREEN_HEIGHT;
            }
            if (e.key.keysym.sym == SDLK_UP) {
                direction = 'w';
            //filled_rect.y = (filled_rect.y + SCREEN_HEIGHT - step) % SCREEN_HEIGHT;
            }
        }
        fill_move(direction);
        updateGrid(direction);
        fill_move(direction);
        if(checkToSpawn() == 1 )spawn();
        //disp(grid);
        for(int r = 0 ; r < cell_num ; r ++)
        {
            for(int c = 0 ; c < cell_num; c++)
            {
                if( grid[c][r] != 0){
                    /*
                    SDL_Rect filled_rect;
                    filled_rect.x = r_pad + r*cell_size;
                    filled_rect.y = c_pad + c*cell_size+70;
                    filled_rect.w = cell_size;
                    filled_rect.h = cell_size;
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // green
                    SDL_RenderFillRect(renderer, &filled_rect);
                    */
                    if( grid[c][r] == 2) renderTexture(image2, renderer, r_pad + r*cell_size, c_pad + c*cell_size+70, cell_size, cell_size);
                    if( grid[c][r] == 4) renderTexture(image4, renderer, r_pad + r*cell_size, c_pad + c*cell_size+70, cell_size, cell_size);
                    if( grid[c][r] == 8) renderTexture(image8, renderer, r_pad + r*cell_size, c_pad + c*cell_size+70, cell_size, cell_size);
                    if( grid[c][r] == 16) renderTexture(image16, renderer, r_pad + r*cell_size, c_pad + c*cell_size+70, cell_size, cell_size);
                    if( grid[c][r] == 32) renderTexture(image32, renderer, r_pad + r*cell_size, c_pad + c*cell_size+70, cell_size, cell_size);
                    if( grid[c][r] == 64) renderTexture(image64, renderer, r_pad + r*cell_size, c_pad + c*cell_size+70, cell_size, cell_size);
                    if( grid[c][r] == 128) renderTexture(image128, renderer, r_pad + r*cell_size, c_pad + c*cell_size+70, cell_size, cell_size);
                    if( grid[c][r] == 256) renderTexture(image256, renderer, r_pad + r*cell_size, c_pad + c*cell_size+70, cell_size, cell_size);
                    if( grid[c][r] == 512) renderTexture(image512, renderer, r_pad + r*cell_size, c_pad + c*cell_size+70, cell_size, cell_size);
                    if( grid[c][r] == 1024) renderTexture(image1024, renderer, r_pad + r*cell_size, c_pad + c*cell_size+70, cell_size, cell_size);
                    if( grid[c][r] == 2048) renderTexture(image2048, renderer, r_pad + r*cell_size, c_pad + c*cell_size+70, cell_size, cell_size);
                }
                else{
                    renderTexture(imageblank, renderer, r_pad + r*cell_size, c_pad + c*cell_size+70, cell_size, cell_size);

                }
            }
            SDL_RenderPresent(renderer);
        }

    }



/*
    while(1){
        spawn();
        disp(grid);
        SDL_Delay(100);
        for(int r = 0 ; r < cell_num ; r ++)
        {
            for(int c = 0 ; c < cell_num; c++)
            {
                if( grid[c][r] != 0){
                    SDL_Rect filled_rect;
                    filled_rect.x = r_pad + r*cell_size;
                    filled_rect.y = c_pad + c*cell_size+70;
                    filled_rect.w = cell_size;
                    filled_rect.h = cell_size;
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // green
                    SDL_RenderFillRect(renderer, &filled_rect);
                }
                else{
                    SDL_Rect filled_rect;
                    filled_rect.x = r_pad + r*cell_size;
                    filled_rect.y = c_pad + c*cell_size+70;
                    filled_rect.w = cell_size;
                    filled_rect.h = cell_size;
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // green
                    SDL_RenderFillRect(renderer, &filled_rect);
                }
            }

        }

    // Dùng lệnh hiển thị (đưa) hình đã vẽ ra mành hình
   //Khi thông thường chạy với môi trường bình thường ở nhà
    SDL_RenderPresent(renderer);

    }
*/



    // Dùng lệnh hiển thị (đưa) hình đã vẽ ra mành hình
    //Khi thông thường chạy với môi trường bình thường ở nhà
    SDL_RenderPresent(renderer);
   //Khi chạy ở máy thực hành WinXP ở trường (máy ảo)
   //SDL_UpdateWindowSurface(window);

   //Đợi 1 phím bất kỳ trước khi đóng cửa sổ
    waitUntilKeyPressed();

    //Giải phóng 2 biến texture đã dùng
    SDL_DestroyTexture(background);

    //Kết thúc SDL (giải phóng các biển window và renderer)
    quitSDL(window, renderer);
    return 0;
}

/**
* Nạp một ảnh bitmap (BMP) vào một texture trên thiết bị hiện thị (rendering device)
* @param file: đường dẫn và tên file ảnh BMP
* @param ren: renderer để nạp texture lên
* @return trả về texture đã nạp, hoặc nullptr nếu có lỗi.
*/
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
{
	//Khởi tạo là nullptr để tránh lỗi 'dangling pointer'
	SDL_Texture *texture = nullptr;
	//Nạp ảnh từ tên file (với đường dẫn)
	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
	//Nếu không có lỗi, chuyển đổi về dạng texture and và trả về
	if (loadedImage != nullptr){
		texture = SDL_CreateTextureFromSurface(ren, loadedImage);
		SDL_FreeSurface(loadedImage);
		//Đảm bảo việc chuyển đổi không có lỗi
		if (texture == nullptr){
			logSDLError(std::cout, "CreateTextureFromSurface");
		}
	}
	else {
		logSDLError(std::cout, "LoadBMP");
	}
	return texture;
}

/**
* Vẽ một SDL_Texture lên một SDL_Renderer tại toạ độ (x, y), trong khi
* giữ nguyên chiều rộng và cao của ảnh
* @param tex: texture nguồn chúng ta muốn vẽ ra
* @param ren: thiết bị renderer chúng ta muốn vẽ vào
* @param x: hoành độ
* @param y: tung độ
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
{
	//Thiết lập hình chữ nhật đích mà chúng ta muốn vẽ ảnh vào trong
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	//Truy vẫn texture để lấy chiều rộng và cao (vào chiều rộng và cao tương ứng của hình chữ nhật đích)
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    //Đưa toàn bộ ảnh trong texture vào hình chữ nhật đích
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

/**
* Vẽ một SDL_Texture lên một SDL_Renderer tại toạ độ (x, y), với
* chiều rộng và cao mới
* @param tex: texture nguồn chúng ta muốn vẽ ra
* @param ren: thiết bị renderer chúng ta muốn vẽ vào
* @param x: hoành độ
* @param y: tung độ
* @param w: chiều rộng (mới)
* @param h: độ cao (mới)
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h)
{
	//Thiết lập hình chữ nhật đích mà chúng ta muốn vẽ ảnh vào trong
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
    dst.w = w;
    dst.h = h;
    //Đưa toàn bộ ảnh trong texture vào hình chữ nhật đích
    //(ảnh sẽ co dãn cho khớp với kích cỡ mới)
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

//*****************************************************
// Các hàm chung về khởi tạo và huỷ SDL
void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
    //   SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);


    //Khi thông thường chạy với môi trường bình thường ở nhà
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    //Khi chạy ở máy thực hành WinXP ở trường (máy ảo)
    //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}
//**************************************************************

