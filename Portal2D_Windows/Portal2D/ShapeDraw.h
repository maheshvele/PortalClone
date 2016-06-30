void DrawCircle(float_t radius, float_t x, float_t y, uchar8_t r, uchar8_t g, uchar8_t b, bool filled);
void DrawLine(float_t startX, float_t startY, float_t endX, float_t endY, uchar8_t r, uchar8_t g, uchar8_t b);
void DrawRect(float_t x, float_t y, float_t width, float_t height, uchar8_t r, uchar8_t g, uchar8_t b);
void DrawCursor();
void DrawStaticLevelObjects();
void DrawPlayer(GLuint texture);
void DrawRayCast();
void DrawPortal(bool isOrangePortal, GLuint texture);
void DrawTexturedRect(GLuint texture, float x, float y, float width, float height, float u1, float u2, float v1, float v2);
void DrawTexturedRectRotate(GLuint texture, float x, float y, float width, float height, float u1, float u2, float v1, float v2);
void DrawPlayerArm(GLuint texture, float x, float y, float width, float height, float u1, float u2, float v1, float v2);
void DrawEnemy();
#define PORTAL_STRETCH 20
