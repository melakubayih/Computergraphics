
#include <GL/glut.h>					// (or others, depending on the system in use)
#include <bevgrafmath2017.h>

int change = 1;

GLsizei winWidth = 800, winHeight = 600;
vec2 points[4] = { {350.0f, 300.0f}, {550.0f, 300.0f}, {400.0f, 500.0f},{180.0f, 190.0f} };

GLint dragged = -1;
int count = 0;

void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, winWidth, 0.0, winHeight);
	glShadeModel(GL_FLAT);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(5.0);
	glLineWidth(1.0);
}
void drawHermite() {
	glColor3f(0, 0, 0);

	glBegin(GL_POINTS);
	for (int i = 0; i < 4; i++)
		glVertex2f(points[i].x, points[i].y);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(points[2].x, points[2].y);
	glVertex2f(points[0].x, points[0].y);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(points[1].x, points[1].y);
	glVertex2f(points[3].x, points[3].y);
	glEnd();
	mat24 G = { points[0],  points[1],   points[2], points[3] - points[0] };

	float t[4] = { 0.0f, 1.0f, 2.0f, 0.0f };
	vec4 columns[4];

	for (int i = 0; i < 4; i++) {
		columns[i] = vec4(t[i] * t[i] * t[i], t[i] * t[i], t[i], 1);
	}
	columns[3] = vec4(3 * t[3] * t[3], 2 * t[3], 1, 0);

	mat4 M = { columns[0],  columns[1],  columns[2],  columns[3], true };
	mat4 M_ = inverse(M);
	mat24 C = G * M_;
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (float t = 0; t <= 1.0; t += 0.01) {
		vec4 T = { t * t * t, t * t, t, 1 };
		vec2 curvePoint = C * T;
		glVertex2f(curvePoint.x, curvePoint.y);
	}
	glEnd();
}

GLint getActivePoint1(vec2 p[], GLint size, GLint sens, GLint x, GLint y)
{
	GLint i, s = sens * sens;
	vec2 P = { (float)x, (float)y };

	for (i = 0; i < size; i++)
		if (dist2(p[i], P) < s)
			return i;
	return -1;
}

void processMouse(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
	GLint i;
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
	{
		if (count == 0)
		{
			i = getActivePoint1(points, 4, 8, xMouse, winHeight - yMouse);
		}
		if (count == 1)
		{
			i = getActivePoint1(points, 4, 8, xMouse, winHeight - yMouse);
		}

		if (i != -1)
		{
			dragged = i;
		}
	}
	if (button == GLUT_LEFT_BUTTON && action == GLUT_UP)
	{
		dragged = -1;
	}
}

void processMouseActiveMotion(GLint xMouse, GLint yMouse)
{
	GLint i;
	if (dragged >= 0) {
		if (count == 0)
		{
			points[dragged].x = xMouse;
			points[dragged].y = winHeight - yMouse;
			glutPostRedisplay();
		}

		if (count == 1)
		{
			points[dragged].x = xMouse;
			points[dragged].y = winHeight - yMouse;
			glutPostRedisplay();
		}

	}
}
void drawHermiteWithoutTLine()
{
	glColor3f(0, 0, 0);

	glBegin(GL_POINTS);
	for (int i = 0; i < 4; i++)
		glVertex2f(points[i].x, points[i].y);
	glEnd();

	mat24 G = { points[0],  points[1],   points[2],  points[3] };

	float t[4] = { -1.0f, 0.0f, 1.0f, 2.0f };

	vec4 columns[4];

	for (int i = 0; i < 4; i++) {
		columns[i] = vec4(t[i] * t[i] * t[i], t[i] * t[i], t[i], 1);
	}

	mat4 M = { columns[0],  columns[1],  columns[2],  columns[3], true };
	mat4 M_ = inverse(M);
	mat24 C = G * M_;

	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (float t = -1; t <= 2.0; t += 0.01) {
		vec4 T = { t * t * t, t * t, t, 1 };
		vec2 curvePoint = C * T;
		glVertex2f(curvePoint.x, curvePoint.y);
	}

	glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	if (change == 1)
	{
		drawHermite();
	}
	if (change == -1)
	{
		drawHermiteWithoutTLine();
	}

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	case 32:
		change *= -1;
		break;
	}
	glutPostRedisplay();
}
int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Hermite Curve");
	init();
	glutDisplayFunc(display);
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseActiveMotion);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}

