# scenic_tourism_information_management_platform
主要功能模块如下：
1、注册登录模块：登录对象包括平台管理人员，景区工作人员和游客。其中管理人员已有默认的账户，工作人员和游客若初次登录需要注册。任何对象在进入平台后若要完成相应操作必须登录，如游客在进入平台后进行购票退票等操作必须登录，若无账户则需要创建账户成为会员。游客和工作人员在创建账户的同时需要提供密保信息，并将账户信息存储到相应的数据库表中。工作人员和游客能够修改账户相关信息和密码，在登录失败的情况下根据密保信息重新设置密码。
2、景点信息管理模块：景区中景点和活动事件的相关信息通过建立合适的数据库模型进行存储。该模块下，管理人员能够进行景点基本信息和活动事件的查看和修改。
3、道路铺设模块：景区可以规划铺设新的道路推进景区交通建设，道路的铺设将直接影响到景区内景点间的路径信息。该模块下设有道路预览和道路管理两个功能。道路预览功能能够按一定算法根据实际情况推荐铺设方案，道路管理功能能够根据实际情况对已铺设的道路进行封路的处理。
4、售票管理模块:该模块下主要完成对游客的购票和退票行为的处理。管理人员还能够通过限制游客人数来控制流量，并且可以查看景区收入等详细信息。
5、人员管理模块:该模块下管理人员能够查看修改景区中的工作人员和游客信息；平台服务人员也能够为游客更换导游，更换已购买的服务；导游能够对选择自己的游客进行查看和管理，也能查看到旅行过程中游客反馈的常见问题。
6、购票退票模块:该模块下游客可以通过购票界面进行景点信息、景点活动事件和导游的查询和选择，进行预约。也可以通过退票界面对已经购票进行退票的操作。
7、服务咨询模块:该模块置于工作人员和游客界面中。主要完成游客的咨询工作。游客既可以通过该界面下的查看常见问题FAQ尝试解决问题，也可以进行留言与工作人员进行交流。工作人员在该界面下，能够及时处理留言问题并上传游客反馈的问题。
8、查询推荐模块:该模块下不需要游客进行登录操作，查询功能界面的菜单栏下提供多种查询方式：用户可以通过景点名称进行查找；也可以通过对景区活动信息进行查找，返回该活动相关信息和与该活动相关联的景点的介绍。查找方式支持完全匹配和任意匹配的模糊查找。推荐功能下利用数据结构图结构相关算法，将景点和相关活动按不同权值排序呈现给游客，并为游客推荐合适的观景顺序。
9、界面设计：该平台使用MVC的设计模式，利用QT技术所带来的前后端分离,能够最大程度降低界面设计难度。在界面设计时，平台的界面必须要美观、便捷。用户在进入平台时首页为登录界面，提供三种角色的身份登录。登录成功后对应三个不同的界面，使不同角色能够在相应的界面上进行操作。

主要技术如下：
C++基础、
Qt5、
SQLite、
Socket编程、
图数据结构及其基本算法。

