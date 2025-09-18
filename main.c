
#define INPUT_TYPE_MOUSE 1
#define INPUT_TYPE_KEYBOARD 2

typedef struct {
    int button;
    int posX;
    int posY;
} MouseInput;

typedef struct {
    int key;
} KeyboardInput;

typedef struct {
    int type;
    union {
        MouseInput mi;
        KeyboardInput ki;
    };
} INPUT;

int main() {

    INPUT input;
    input.type = INPUT_TYPE_KEYBOARD;
    input.mi.

    return 0;
}

