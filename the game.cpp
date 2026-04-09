#include <iostream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <string>
using namespace std;
class Game2048 {
private:
    int board[4][4];
    int score = 0;
    int origin[16];
    int change[16];
    void addRandomTile() {
        vector<pair<int, int>> emptyTiles;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (board[i][j] == 0) emptyTiles.push_back({ i, j });
            }
        }
        if (!emptyTiles.empty()) {
            int idx = rand() % emptyTiles.size();
            board[emptyTiles[idx].first][emptyTiles[idx].second] = (rand() % 10 == 0) ? 4 : 2;
        }
    }
    bool compressAndMerge(int row[4]) {
        // TODO: Step 1 - First Compression (Move everything to the left)
        bool merge = false;
        vector<int> num = { 0,0,0,0 };
        int n = 0;
        for (int i = 0; i < 4; i++) {
            if (row[i] != 0) {
                num[n++] = row[i];
            }
        }
        // TODO: Step 2 - Merge adjacent equal tiles (e.g., [2, 2, 0, 0] -> [4, 0, 0, 0])
        // Hint: Don't forget to add the merged value to 'score'
        for (int i = 0; i < 3; i++) {
            if (num[i] == num[i + 1]) {
                merge = true;
                num[i] += num[i + 1];
                score += num[i];
                num[i + 1] = 0;
                i++;
            }
        }
        // TODO: Step 3 - Second Compression (Fill gaps created by Step 2)
        int j = 0;
        for (int i = 0; i < 4; i++) {
            if (num[i] != 0) {
                swap(num[i], num[j++]);
            }
        }
        // TODO: Step 4 - Update the original row and set 'changed' flag
        for (int i = 0; i < 4; i++) {
            row[i] = num[i];
        }
        return merge;
    }
public:
    Game2048() {
        srand(static_cast<unsigned int>(time(0)));
        clear();
    }
    void clear() {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                board[i][j] = 0;
        score = 0;
    }
    void inputBoard() {
        clear(); // 输入前先清空旧数据
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                cin >> board[i][j];
    }
    void draw() {
        cout << "\n-----------------------------" << endl;
        cout << "      Score: " << score << endl;
        cout << "-----------------------------" << endl;
        for (int i = 0; i < 4; ++i) {
            cout << "|";
            for (int j = 0; j < 4; ++j) {
                if (board[i][j] == 0) cout << "      |";
                else cout << setw(5) << board[i][j] << " |";
            }
            cout << "\n-----------------------------" << endl;
        }
    }
    bool move(char direction, bool addRandom = true) {
        bool changed = false;
        // TODO: Handle 'a' (Move Left)
        // Action: Process each row board[i] directly.
        if (direction == 'a') {
            for (int i = 0; i < 4; i++)
            {
                if (compressAndMerge(board[i])) {
                    changed = true;
                }
            }
        }
        // TODO: Handle 'd' (Move Right)
        // Action: Extract row -> Reverse -> compressAndMerge -> Reverse back -> Store.
        if (direction == 'd') {
            for (int i = 0; i < 4; i++)
            {
                int row[4] = { board[i][3],board[i][2],board[i][1],board[i][0] };
                if (compressAndMerge(row)) {
                    changed = true;
                    board[i][0] = row[3]; board[i][1] = row[2];
                    board[i][2] = row[1]; board[i][3] = row[0];
                }
            }
        }
        // TODO: Handle 'w' (Move Up)
        // Action: Extract column -> compressAndMerge -> Store back to column.
        if (direction == 'w') {
            for (int i = 0; i < 4; i++) {
                int row[4] = {board[0][i],board[1][i],board[2][i],board[3][i]};
                if (compressAndMerge(row)) {
                    changed = true;
                    board[0][i] = row[0]; board[1][i] = row[1];
                    board[2][i] = row[2]; board[3][i] = row[3];
                }
            }
        }
        // TODO: Handle 's' (Move Down)
        // Action: Extract column -> Reverse -> compressAndMerge -> Reverse back -> Store.
        if (direction == 's') {
            for (int i = 0; i < 4; i++) {
                int row[4] = { board[3][i],board[2][i],board[1][i],board[0][i] };
                if (compressAndMerge(row))
                {
                    changed = true;
                    board[3][i] = row[0]; board[2][i] = row[1];
                    board[1][i] = row[2]; board[0][i] = row[3];
                }
            }
        }
        if (changed && addRandom) addRandomTile();
        return changed;
    }
    void output() {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j)
                cout << board[i][j] << (j == 3 ? "" : " ");
            cout << endl;
        }
        cout << "Score: " << score << endl;
    }

    bool checkGameOver() {
        /* TODO: check whether the game is over or not*/
        bool over = false;
        bool check = true;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (board[i][j] == 2048) { over = true; }
            }
        }
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == board[i][j + 1]) {
                    check = false;
                }
            }
        }
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (board[i][j] == board[i + 1][j]) {
                    check = false;
                }
            }
        }
        if (!check) { over = true; }
        return over;
    }

    void gameStart() {
        clear();
        addRandomTile();
        addRandomTile();
    }
};

// --- 模式函数 ---

void play() {
    Game2048 game;
    game.gameStart(); // 初始化两个随机块
    char input;
    cout << "Welcome to 2048! (WASD move, Q quit)" << endl;
    game.draw();
    while (cin >> input) {
        if (tolower(input) == 'q') break;
        if (game.move(input, true)) { // 游戏模式开启随机生成
            game.draw();
            if (game.checkGameOver()) {
                cout << "GAME OVER!" << endl;
                break;
            }
        }
    }
}
void test() {
    Game2048 game;
    game.inputBoard();
    string ops;
    if (!(cin >> ops)) return;
    for (char c : ops) {
        game.move(c, false); // OJ模式关闭随机生成
    }
    game.output();
}
int main() {
    // 根据需要切换注释即可：
    test();  // 交互模式，如果想进行游戏的话记得切换到play()这里是作业的test()模式
    // test();  // OJ 模式
    return 0;
}
