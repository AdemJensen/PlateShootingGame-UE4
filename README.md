# PlateShootingGame
使用 UE4 开发的一款~~打靶~~混战小游戏，腾讯游戏开发公开课作业2~6。

## 更新记录
- Update 2020-12-18:
    - 添加两个AI：步枪手AI和狙击手AI，二者具有不同的行为树，在战略上各不相同。
    - 对局中固定添加8名AI选手与真人玩家共同游戏。AI玩家将会互相混战。
    - 地图中新增两种武器：狙击步枪、RPG火箭筒。
    - 新增开镜系统，在PC端可以通过鼠标右键使用AR-15（突击步枪）和AW-50（狙击步枪）的准具。
    - 新增伤害方向指示系统，受到攻击后将指示攻击的方向。
    - 在移动端新增游戏菜单系统，能够正确退出游戏。
    - 新增游戏对局内的比分面板，在PC端可以通过按住Tab键呼出，在移动端可以通过点按比分栏呼出。
    - 修正主HUD滑动操作板，修正滑动操作板在UI上的层级关系，优化操作手感。
    - 平衡性调整：突击步枪腰射准确度降低（偏移0° --> 偏移3°），使用准具将没有偏移。
    - 其他：各种BUG修复。
- Update 2020-12-11:
    - 完成联网系统，修复联网玩家名称不正确的BUG。
    - 新增平台类型判断系统，根据平台的不同显示不同的操作界面。
    - 为移动平台优化开火按键，现在移动端的开火按键可以移动，实现右手便开火边转视角。
    - 完成角色类、武器类C++化。
    - 击中敌方角色将会产生白色击中准星，击杀敌方角色将会产生红色击中准星。
    - 新增顶部得分显示和击杀数目标显示，显示当前自己的击杀数和场上最高击杀数。
    - 游戏目标已更改，目前时间固定为10分钟，击杀数目标固定为20。
    - 新增弹药指示器，玩家弹药少于40%后将自动显示地图上弹药的位置。
    - 另：榴弹枪还没有适配新的武器类，暂时被移除，将在后续版本被加入。
    - 飞行靶子正式成为历史了，估计后面学了AI以后会加入比较厉害的AI角色。
- Update 2020-11-27：
    - 完成武器系统，新增射线枪（突击步枪）、榴弹枪，支持 Pick 和 Drop
    - 射线枪、榴弹枪子弹均有物理冲击效果。榴弹枪子弹爆炸后能对周围产生冲击，将周围的物体和角色炸飞。
    - 榴弹枪子弹飞行途中可以被射线抢和爆炸冲击摧毁，摧毁时将爆炸。
    - 角色布娃娃系统修复，死亡后开启布娃娃系统更加真实。
    - 物理材质系统启用，子弹击中不同材质将会有不同的音效和粒子效果。
    - 爆头系统启用，击中头部将造成更多伤害。
    - 新增人形靶子：Kaoru，对其造成不同量的伤害能听到不同的受伤音效。
    - Emmmmmm，因为飞行靶子跟作业无关，暂时屏蔽了，想开的话开个选项就完事了
- Update 2020-11-20：完成了人物动作动画。
- Update 2020-11-06：完成了手机端操作按钮及UI，添加排行榜、昵称修改面板等。

## 资源链接
- 演示视频：[https://share.weiyun.com/6Tqo12yu](https://share.weiyun.com/6Tqo12yu)
- 安装包-安卓：[https://share.weiyun.com/X1wOLNmQ](https://share.weiyun.com/X1wOLNmQ)
- 安装包-Windows：[https://share.weiyun.com/BJnOhjbt](https://share.weiyun.com/BJnOhjbt)

提示：已知在新版Edge浏览器（Chromium内核版）上播放演示视频可能显示全部黑屏，请更换其他浏览器（已测试IE、Firefox、Chrome均正常），或者直接下载视频观看，给您带来的不便还请谅解。

## 简介
本游戏目前已经修改为多人混战游戏。

服务器开启游戏房间后，其他玩家可自由加入，加入后即各自为战。时间限定10分钟，在规定时间内完成指定数量击杀目标即可获得胜利。

地图上除了最多支持8名真人玩家同时游戏外，还固定放置了8名电脑玩家（4名突击步枪手+4名狙击步枪手），AI各自为战。

玩家进入地图后没有武器，需要在地图上拾取武器。子弹耗尽后，可以选择更换武器，也可以到地图上指定的标记点进行弹药补给。

目前地图上的武器：
- AR-15（突击步枪）若干把，分布在地图各处。
- AW-50（狙击步枪）4把，分布在地图的四个角落。另外，击杀AI狙击手也能够获得其手中的狙击步枪。
- RPG（火箭发射器）4把，分布在地图三条线路的中部位置，可对敌人造成大范围爆炸伤害。注意，小心使用，自己可能也会受伤哦！

角色拖动左侧摇杆可以移动位置，射击按钮射击，瞄准按钮使用准具（如果武器支持），跳跃按钮跳跃。走到可拾取物品周围时，左侧将会显示其信息和按钮，点击即可拾取。

## 关于桌面版
新版本适配了桌面版的操作和界面，你甚至可以让桌面版和手机版一起联机游戏。

（有堡垒之夜内味了hhhh）