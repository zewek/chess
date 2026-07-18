#include "glad.h"
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <string>
#include "backend.h"
using namespace std;

struct pos {
    int x, y;
};
pos lift_piece_pos;
chessboard board;
bool lift_piece=false;
string lift_piece_text;
chesscolour lift_piece_colour;

//#define TEST

int main() {
    // 1. 基础窗口初始化
    glfwInit();
    GLFWwindow *window=glfwCreateWindow(600, 900, "POKER CHESS", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // 2. 初始化 ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io=ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    //初始化字体
    ImFont *chess_font=nullptr;
    chess_font=io.Fonts->AddFontFromFileTTF("FiraMonoNerdFont-Bold.otf", 50.0f);
    if (chess_font==nullptr) {
        printf("loading OTF failed");
        return 1;
    }
    
    // 3. 主循环
    while (!glfwWindowShouldClose(window)) {
        //glfwPollEvents();
        glfwWaitEvents();

        // 开启新的一帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //自适应Windows窗口
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        ImGui::SetNextWindowSize(ImVec2((float)display_w, (float)display_h));
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGuiWindowFlags window_flags=
            ImGuiWindowFlags_NoTitleBar |      // 去掉标题栏
            ImGuiWindowFlags_NoResize |        // 禁止手动缩放（由外层决定）
            ImGuiWindowFlags_NoMove |          // 禁止拖拽移动
            ImGuiWindowFlags_NoCollapse |      // 禁止折叠
            ImGuiWindowFlags_NoBringToFrontOnFocus| // 避免遮挡其他可能存在的子窗口
            ImGuiWindowFlags_NoBackground; // 去掉灰色背景，使它透明（露出 OpenGL 的背景色）
        ImGui::Begin("POKER CHESS", nullptr, window_flags);
        //判定胜负
        if (board.king_sum(RED)==0 or board.king_sum(BLUE)==0) {
            ImGuiWindowFlags window_flags=
                ImGuiWindowFlags_NoResize |        // 禁止手动缩放（由外层决定）
                ImGuiWindowFlags_NoMove |          // 禁止拖拽移动
                ImGuiWindowFlags_NoCollapse;      // 禁止折叠
            ImGui::Begin("NOTE");
            ImGui::Text("GAME OVER");
            if (ImGui::Button("OK")) exit(0);
            ImGui::End();
        }
        //绘制棋盘
        for (int i=1; i<=6; ++i) {
            for (int j=1; j<=6; ++j) {
                ImGui::PushID(i*10+j);
                ImGui::SetCursorPos(ImVec2((j-1)*100, (i-1)*150)); //列，行
                string text=board.get_text(i, j);
                //更改颜色
                if (board.get_colour(i, j)==GREY) {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.55f, 0.35f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
                } else if (board.get_colour(i, j)==RED) {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
                } else {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.8f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.5f, 0.9f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.3f, 0.7f, 1.0f));
                }
                if (ImGui::Button(text.c_str(), ImVec2(100.0f, 150.0f))) {
                    if (lift_piece) {
                        if (lift_piece_pos.x==i and lift_piece_pos.y==j) {
                            lift_piece=false;
                        } else if (board.check(lift_piece_pos.x, lift_piece_pos.y, i, j)) {
                            //printf("%d %d\n", board.king_sum(RED), board.king_sum(BLUE));
                            board.move(lift_piece_pos.x, lift_piece_pos.y, i, j);
                            lift_piece=false;
                        }
                    } else {
                        lift_piece=true;
                        lift_piece_text=board.get_text(i, j);
                        lift_piece_colour=board.get_colour(i, j);
                        lift_piece_pos.x=i, lift_piece_pos.y=j;
                    }
                }
                //pop颜色
                ImGui::PopStyleColor(3);
                ImGui::PopID();
            }
        }
        if (lift_piece) {
            ImGui::SetCursorPos(ImVec2((lift_piece_pos.y-1)*100, (lift_piece_pos.x-1)*150));
            if (lift_piece_colour==RED) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
                ImGui::Button(lift_piece_text.c_str(), ImVec2(100.0f, 150.0f));
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.3f, 0.7f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.15f, 0.3f, 0.7f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.3f, 0.7f, 1.0f));
                ImGui::Button(lift_piece_text.c_str(), ImVec2(100.0f, 150.0f));
            }
            ImGui::PopStyleColor(3);
        }
        ImGui::End();

        // 4. 渲染
        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT); // 清屏（默认黑色背景）
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // 5. 清理
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}
