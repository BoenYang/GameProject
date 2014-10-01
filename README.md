##GameProject##


##团队成员
+ 老吴
+ 小博
+ 黄兄 
+ 我是坏人，啊哈哈~~//

##TO DO
+ 分辨率适配
+ 声音
+ 交互设计
+ 关卡难度降低
+ 添加道具
+ 某些界面

##关于工程构成
+ cutegine 游戏切割引擎
+ utils    游戏工具类
+ elements 游戏元素
+ scene    游戏场景


##关于关卡设计

>写了个关卡的配置文件编写规则，使用文件来保存关卡数据，之后还有一些更新 

示例如下
```xml 
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd"> 
<plist version="1.0">
	<dict>
		<key>Level</key>
		<array>
			<dict>
				<key>FileName</key><string>elements/wood120x30.png</string>
				<key>Position</key><string>{275,710}</string>
				<key>Rotation</key><integer>90</integer>
				<key>Type</key><string>wood</string>
			</dict>
			<dict>
				<key>FileName</key><string>elements/wood120x30.png</string>
				<key>Position</key><string>{185,710}</string>
				<key>Rotation</key><integer>90</integer>
				<key>Type</key><string>wood</string>
			</dict>
			<dict>
				<key>FileName</key><string>elements/wood120x30.png</string>
				<key>Position</key><string>{230,785}</string>
				<key>Rotation</key><integer>0</integer>
				<key>Type</key><string>wood</string>
			</dict>
			<dict>
				<key>FileName</key><string>elements/surface500x20.png</string>
				<key>Position</key><string>{350,640}</string>
				<key>Rotation</key><integer>0</integer>
				<key>Type</key><string>surface</string>
			</dict>
			<dict>
				<key>FileName</key><string>elements/hero60x60.png</string>
				<key>Position</key><string>{230,835}</string>
				<key>Rotation</key><integer>0</integer>
				<key>Type</key><string>hero</string>
				<key>Radius</key><integer>30</integer>
			</dict>
			<dict>
				<key>FileName</key><string>elements/bomb60x60.png</string>
				<key>Position</key><string>{500,680}</string>
				<key>Rotation</key><integer>0</integer>
				<key>Type</key><string>bomb</string>
				<key>Radius</key><integer>30</integer>
			</dict>
			</dict>
				<key>FileName</key><string>label/star.png</string>
				<key>Position</key><string>{500,680}</string>
				<key>Type</key><string>star</string>
			</dict>
		</array>
		<key>Metadata</key>
		<dict>
			<key>BodyPlist</key><string>BodyPlist.plist</string>
		</dict>
	</dict>
</plist>
```
```
int main()
{
	
}
```