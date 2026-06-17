# *不好，怪物入侵了我们的村庄！快组织小伙伴们架枪起炮，赶走它们！*

村庄周围正在逐波次地刷新怪物，在怪物的最后一波攻势结束前，尽全力包围村庄！

        场上的所有怪物都被击败或进入目标点后，如果还有剩余生命值，则游戏胜利，否则游戏失败

<img width="149" height="65" alt="image" src="https://github.com/user-attachments/assets/88ff2bfa-f2af-4869-b64f-0f75f71e7576" />

- 房子：保护目标点，

- 心：剩余心的数量=剩余生命值，

- 金币：在放置防御塔消耗对应数量金币，金币不足无法放置。

  金币获取方式：
  
      金币会随时间自动增加
      击杀怪物概率掉落金币，这时将鼠标移动到金币上即可获取金币。当雷电龙碰到金币时，也会捡起它。注意！掉在地上的的金币过一段时间会自动消失
      每个波次结束也会有大量金币奖励

<img width="144" height="144" alt="ui_place_idle" src="https://github.com/user-attachments/assets/c21aeb7c-c585-482a-8c5d-1517207f734c" />
<img width="144" height="144" alt="image" src="https://github.com/user-attachments/assets/03cfba9e-ab31-4a1d-a0b8-d800ca827d58" />

        鼠标左键/右键点击 敌人行动路径之外 的地图格，轮盘下方会显示建造防御塔所需的金币数
        在金币足够时 拖拽鼠标 至三种防御塔之一，从而放置防御塔
        
        你也可以点击保护目标点，从而升级防御塔！

- 防御塔种类：
1. <img width="48" height="47" alt="image" src="https://github.com/user-attachments/assets/9522ce0c-d94b-489c-aba3-42be3b20130f" />弓箭手：单体伤害
2. <img width="45" height="39" alt="image" src="https://github.com/user-attachments/assets/d9ed1802-9c7d-4274-8def-c956bd7d2875" />斧子手：单体减速
3. <img width="50" height="46" alt="image" src="https://github.com/user-attachments/assets/d0643fd8-e2a3-4085-a498-d7e8d87d58ec" />炮手：范围爆炸伤害

- 玩家键盘控制：
<img width="65" height="65" alt="ui_player_avatar" src="https://github.com/user-attachments/assets/f8fe878e-d68c-4a23-8a27-057e3dd669a4" /> 

        按键盘上的WASD控制雷电龙移动

<img width="234" height="57" alt="image" src="https://github.com/user-attachments/assets/24d3a808-c805-4acf-8da3-4c6c967c33c8" /> **冷却条**

        按 J 进行普攻对前方直线内敌人造成群体伤害，

        按 K 释放特殊技能(范围减速，需要一定时间回复，左上角冷却条回满时，可以释放技能)

        按 ESC 暂停游戏

- 敌人种类

1. <img width="81" height="71" alt="image" src="https://github.com/user-attachments/assets/2ad144b5-c152-44c4-ac4c-47fef49b6e0c" />史莱姆：大量出现，进入保护目标点扣1生命值

2. <img width="84" height="73" alt="image" src="https://github.com/user-attachments/assets/4b0d736c-2f31-43b6-aabb-f95f5f9cd368" />皇家史莱姆：史莱姆的强化版，移速，血量都得到了提升

3. <img width="92" height="88" alt="image" src="https://github.com/user-attachments/assets/55591a6d-3565-4f8f-9134-ccdd3d845e1a" />骷髅：移动速度快

4. <img width="81" height="86" alt="image" src="https://github.com/user-attachments/assets/f9fa9377-29a3-4d7a-a19b-6de575d5dd11" />哥布林：进入保护目标点扣2生命值

5. <img width="86" height="77" alt="image" src="https://github.com/user-attachments/assets/1406a451-65d7-4a9d-a642-61be8b458bb0" />哥布林祭司：血量高，移动速度慢，进入保护目标点扣2生命值，拥有为一定范围内的敌人回血(对自身治疗效果加强)




# c++双人合作塔防游戏！
*本人学习B站* ***voidmatrix*** *大大《从零开始的 C++双人塔防游戏！》学习编写的练习项目，目的为理解一般的游戏架构，理解引游戏擎底层逻辑*
## 开篇：遇到的困难与解决困难（配置环境）
SDL库版本问题/visual studio版本问题：
（2026年SDL2已经更新到了SDL3，visual studio2022更新到了visual studio 2026）
- 使用SDL3，不可避免地遇到了SDL2/SDL3函数名，函数参数，宏定义不一致等等问题。
- 以及sdl3_gfx需要手动编译并生成.ib文件
## 中段：写写写配置文件
- 利用cJSON库，从json文件里解析配置数据
## 中后：单独创建新的T_manager类
- 为了解决SDL3和SDL2的MIX播放类函数使用不一致的问题，我开始新建额外的单例类audio_manger
- 我逐渐理解一切。震撼寰宇，我已经完全理解了各个头文件的组织方式与使用时机
## 后期：优化
- 不可思议，游戏已经可以正常跑完一个完整的流程了
- 自行添加：暂停游戏，鼠标移动到金币上时直接自动捡金币 等功能
- 已开发功能：
  
  1.  暂停游戏时的横幅
  
  2. **重载关卡**，每关结束后自动进入下一关
  
  3.  教学关卡，指导玩家使用按键，与各种物件交互
  
      新增地图现有地图3张
  
  4.  release版本
 
- 待开发功能：
  1.  暂停时可以放置防御塔
  2.  菜单界面，可以自由选择关卡并看到地图缩略图等
