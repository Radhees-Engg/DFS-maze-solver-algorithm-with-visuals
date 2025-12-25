#include <iostream>
#include <raylib.h>
#include <stack>
#define ROWS 10
#define COLS 10
using namespace std;

const int SIZE = 10, BOT = 1, END = 9, WALL = 8, VISITED = 2, DEAD_END = 3, BOT_SIZE = 5;

Vector2 getBotPixel(int grid_x, int grid_y, int cell_w, int cell_h) 
{
	return { grid_y * cell_w + cell_w / 2.0f, grid_x * cell_h + cell_h / 2.0f };
}
void Draw_Maze(int cell_width, int cell_height, bool& maze_solved, int& bot_pos_x, int& bot_pos_y, int(*maze)[SIZE])
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (maze[i][j] == WALL)
			{
				DrawRectangle(j * cell_width, i * cell_height, cell_width, cell_height, BLUE);
			}
			if (maze[i][j] == END)
			{
				DrawRectangle(j * cell_width, i * cell_height, cell_width, cell_height, PINK);
			}
			if (maze[i][j] == VISITED && maze[i][j] != END)
			{
				DrawRectangle(j * cell_width, i * cell_height, cell_width, cell_height, GREEN);
			}
			if (maze[i][j] == DEAD_END)
			{
				DrawRectangle(j * cell_width, i * cell_height, cell_width, cell_height, RED);
			}
			DrawRectangleLines(j * cell_width, i * cell_height, cell_width, cell_height, BLACK);
		}
	};
	Vector2 bot_pix = getBotPixel(bot_pos_x, bot_pos_y, cell_width, cell_height);
	DrawCircleV(bot_pix, BOT_SIZE, GRAY);
}
bool is_valid(int x, int y, int(*maze)[SIZE])
{
	return x >= 0 && x < COLS && y >= 0 && y < ROWS && maze[x][y] != WALL;
}
bool solved(int x, int y, int end_x,int end_y)
{
	return (x == end_x && y == end_y);
}
void back_track(int& x, int& y, int& end_x, int& end_y, bool& maze_solved, stack<Vector2>& memory, int(*maze)[SIZE])
{
	if (memory.empty()) return;
	maze[x][y] = DEAD_END;
	auto prev_ele = memory.top();
	memory.pop(); 
	x = (int)prev_ele.x; y = (int)prev_ele.y;
}
void solve(int& x, int& y, int& end_x, int& end_y, bool& maze_solved, stack<Vector2>& memory, int(*maze)[SIZE])
{
	if (solved(x, y, end_x, end_y))
	{
		maze_solved = true;
		return;
	}
	int dx[4] = { 1,-1,0,0 };
	int dy[4] = { 0,0,1,-1 };
	int nx = x; int ny = y;
	for (int d = 0; d < 4; d++)
	{
		nx = x + dx[d];
		ny = y + dy[d];
		if (is_valid(nx, ny, maze) && maze[nx][ny] != VISITED && maze[nx][ny] != DEAD_END )
		{
			Vector2 pre_pos = { x,y };
			memory.push(pre_pos);
			maze[x][y] = VISITED;
			x = nx; y = ny;
			return;
		}		
	}
	back_track(x, y, end_x, end_y, maze_solved, memory, maze);

}


int main()
{
	const int screen_width = 500, screen_height = 500;
	const int cell_width = screen_width / ROWS;
	const int cell_height = screen_height / ROWS;
	bool maze_solved = false;

	int start_pos_x = 0, start_pos_y = 0, end_pos_x = 0, end_pos_y = 0;
	stack<Vector2>memory;
	int maze[SIZE][SIZE] =
	{
	  {1, 0, 8, 8, 8, 8, 8, 8, 9, 8},
      {8, 0, 0, 0, 0, 0, 8, 0, 0, 8},
      {8, 8, 8, 8, 8, 0, 8, 0, 8, 8}, 
      {0, 0, 0, 0, 8, 0, 0, 0, 8, 0},
      {8, 8, 8, 0, 8, 8, 8, 0, 8, 0},
      {8, 0, 0, 0, 0, 0, 8, 0, 0, 0},
      {8, 0, 8, 8, 8, 0, 8, 8, 8, 0},
      {8, 0, 0, 0, 8, 0, 0, 0, 0, 0},
      {8, 8, 8, 0, 8, 8, 8, 8, 8, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (maze[i][j] == BOT)
			{
				start_pos_x = i; start_pos_y = j;
			}
			if (maze[i][j] == END)
			{
				end_pos_x = i; end_pos_y = j;
			}
		}
	}
	
	InitWindow(screen_width, screen_height, "Tryna_Give_Visual_to_my_DFS");
	SetTargetFPS(5);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		Draw_Maze(cell_width, cell_height, maze_solved, start_pos_x, start_pos_y, maze);
		if (!maze_solved)
		{
			solve(start_pos_x, start_pos_y, end_pos_x, end_pos_y, maze_solved, memory, maze);
		}
		EndDrawing();
	}
	CloseWindow();

}