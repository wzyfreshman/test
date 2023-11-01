#ifndef GL_CANONDRAW_H
#define GL_CANONDRAW_H

class GL_GcodeDraw;
class GL_GcodeElement;

#include <QList>
#include <QColor>

class GL_CanonDraw
{
public:
    GL_CanonDraw();
    void canonDraw();
    void drawAxis();
    void drawLimit();
    void drawOrigin();
    void drawModel();
    void drawModelList(bool sampled = false);
    void drawTool();
    void drawSelect();
    virtual void drawRealtimePath();

    void generateToolShape();
    void generateOriginShape();
    void generateAxisShape();


    void flushModel(GL_GcodeDraw *);
    void flushModelList(GL_GcodeDraw *);
    void select(int x,int y);
    void searchFromLineNum(int);
    bool hasSelect();
    void updateSelectBuffer(QList< GL_GcodeElement > *els,
                            int start,int end);
    void updateSelectBuffer(QList< GL_GcodeElement > *els,
                            int i);
    virtual void updateToolCoordinate();
    virtual int getWindowHeight();
    virtual int getWindowWidth();
    virtual void selectLineNum(int);

    static void setColors(const QString &type,float color[4]);
    static void setColors(const QString &type,const QColor &color);

private:
    inline double twoLinesDistance(double *p1,double *p2,
                                   double *p3,double *p4);
    inline double pointToLine(double *p1, double *p2,
                       double *p3);
    inline double lineLength(double *p1, double *p2);
    inline double twoPointAngleToLine(double *p1,double *p2,
                              double *p3,double *p4);

    float *axis;

    unsigned int vboId; //for vbo

    unsigned int listId_tool; //for list
    unsigned int listId_axis; //for list
    unsigned int listId_origin; //for list


    unsigned int listId_feed;       //for list
    unsigned int listId_arcfeed;    //for list
    unsigned int listId_dewells;    //for list
    unsigned int listId_traverse;   //for list

    unsigned int listId_feed_sampled;       //for list
    unsigned int listId_arcfeed_sampled;    //for list
    unsigned int listId_dewells_sampled;    //for list
    unsigned int listId_traverse_sampled;   //for list
    unsigned int sampled_num;


    unsigned int feedsize;
    unsigned int arcfeedsize;
    unsigned int dewellssize;
    unsigned int traversesize;

    float *vectors_feed;
    float *vectors_arcfeed;
    float *vectors_dwells;
    float *vectors_traverse;


protected:


    float objdist;
    GL_GcodeDraw * gcodedraw=nullptr;
    bool sampled;
    bool moving;

    bool has_select;
    int select_count;
    float *vectors_select;

    float machine_limit_min[4];
    float machine_limit_max[4];
    float sec_limit_min[4];
    float sec_limit_max[4];
    float sec_limit_offset[4];

    float g5x_offset[4];
    float g92_offset[4];
    float rotation_xy;

    float tool_pos[4];

    static float color_background[4];
    static float color_tool[4];
    static float color_limit[4];
    static float color_origin[4];
    static float color_feed[4];
    static float color_arcfeed[4];
    static float color_traverse[4];
    static float color_selected[4];
    static float color_rtpath[4];

};

#endif // GL_CANONDRAW_H
