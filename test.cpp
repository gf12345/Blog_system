/*************************************************************************
> File Name: test.cpp
> Author: 
> Mail: 
> Created Time: 2021年01月21日 星期四 08时44分02秒
************************************************************************/

#include"db.hpp"
#include"httplib.h"
using namespace httplib;
blog_system::TableBlog *table_blog;
blog_system::TableTag *table_tag;
void InsertBlog(const Request& req,Response& rsp){
    Json::Reader reader;
    Json::Value blog;
    Json::Value errmsg;
    Json::FastWriter writer;
    bool ret=reader.parse(req.body,blog);
    if(!ret){
        printf("InsertBlog,parse blog json failed!\n");
        rsp.status=400;
        errmsg["ok"]=false;
        errmsg["reason"]="parse blog json failed!";
        //rsp.body=writer.write(errmsg);
        rsp.set_content(writer.write(errmsg),"application/json");
        return;
    }
    ret=table_blog->Insert(blog);
    if(!ret){
        printf("InsertBlog insert into database falied\n");
        rsp.status=500;
        return;
    }
    rsp.status=200;
    return;

}
void DeleteBlog(const Request& req,Response& rsp){
    int blog_id=stoi(req.matches[1]);
    bool ret=table_blog->Delete(blog_id);
    if(!ret){
        printf("DeleteBlog delete from database falied\n");
        rsp.status=500;
    }
    rsp.status=200;
    return;
}
void UpdateBlog(const Request& req,Response& rsp){
    int blog_id=stoi(req.matches[1]);
    Json::Value blog;
    Json::Reader reader;
    bool ret=reader.parse(req.body,blog);
    if(!ret){
        printf("UodateBlog parse json failed\n");
        rsp.status=400;
        return;
    }
    blog["id"]=blog_id;
    ret=table_blog->Update(blog);
    if(!ret){
        printf("UpdateBlog update database falied\n");
        rsp.status=500;
    }
    rsp.status=200;
    return;
}
void GetAllBlog(const Request& req,Response& rsp){
    Json::Value blogs;
    bool ret=table_blog->Getall(&blogs);
    if(!ret){
        printf("GetAllBlog getall database falied\n");
        rsp.status=500;
    }
    Json::FastWriter writer;
    rsp.set_content(writer.write(blogs),"application/json");
    rsp.status=200;
    return;
}
void GetOneBlog(const Request& req,Response& rsp){
    int blog_id=stoi(req.matches[1]);
    Json::Value blog;
    blog["id"]=blog_id;
    bool ret=table_blog->Getone(&blog);
    if(!ret){
        printf("GetOneBlog getone database falied\n");
        rsp.status=500;
    }
    Json::FastWriter writer;
    rsp.set_content(writer.write(blog),"application/json");
    rsp.status=200;
    return;
}
void InsertTag(const Request& req,Response& rsp){
    Json::Reader reader;
    Json::Value tag;
    Json::Value errmsg;
    Json::FastWriter writer;
    bool ret=reader.parse(req.body,tag);
    if(!ret){
        printf("Inserttag,parse tag json failed!\n");
        rsp.status=400;
        errmsg["ok"]=false;
        errmsg["reason"]="parse tag json failed!";
        //rsp.body=writer.write(errmsg);
        rsp.set_content(writer.write(errmsg),"application/json");
        return;
    }
    ret=table_tag->Insert(tag);
    if(!ret){
        printf("InsertTag insert into database falied\n");
        rsp.status=500;
        return;
    }
    rsp.status=200;
    return;
}
void DeleteTag(const Request& req,Response& rsp){
    int tag_id=stoi(req.matches[1]);
    bool ret=table_tag->Delete(tag_id);
    if(!ret){
        printf("DeleteTag delete from database falied\n");
        rsp.status=500;
    }
    rsp.status=200;
    return;
}
void UpdateTag(const Request& req,Response& rsp){
    int tag_id=stoi(req.matches[1]);
    Json::Value tag;
    Json::Reader reader;
    bool ret=reader.parse(req.body,tag);
    if(!ret){
        printf("Uodatetag parse json failed\n");
        rsp.status=400;
        return;
    }
    tag["id"]=tag_id;
    ret=table_tag->Update(tag);
    if(!ret){
        printf("UpdateTag update database falied\n");
        rsp.status=500;
    }
    rsp.status=200;
    return;
}
void GetAllTag(const Request& req,Response& rsp){
    Json::Value tags;
   bool ret=table_tag->Getall(&tags);
    if(!ret){
        printf("GetAllTag getall database falied\n");
        rsp.status=500;
    }
    Json::FastWriter writer;
    rsp.set_content(writer.write(tags),"application/json");
    rsp.status=200;
    return;
}
void GetOneTag(const Request& req,Response& rsp){
   int tag_id=stoi(req.matches[1]);
    Json::Value tag;
    tag["id"]=tag_id;
    bool ret=table_tag->Getone(&tag);
    if(!ret){
        printf("GetOneTag getone database falied\n");
        rsp.status=500;
    }
    Json::FastWriter writer;
    rsp.set_content(writer.write(tag),"application/json");
    rsp.status=200;
    return;
}
int main()
{
    MYSQL* mysql=blog_system::MysqlInit();
    table_blog=new blog_system::TableBlog(mysql);
    table_tag=new blog_system::TableTag(mysql);
    //业务处理
    Server server;
    server.set_base_dir("./www");
    //博客的插入
    server.Post("/blog",InsertBlog);
    //博客的删除
    server.Delete(R"(/blog/(\d+))",DeleteBlog);
    //博客的修改
    server.Put(R"(/blog/(\d+))",UpdateBlog);
    //获取博客信息
    //所有
    server.Get("/blog",GetAllBlog);
    //单个
    server.Get(R"(/blog/(\d+))",GetOneBlog);
    //标签的插入
    server.Post("/tag",InsertTag);
    //标签的删除
    server.Delete(R"(/tag/(\d+))",DeleteTag);
    //标签的修改
    server.Put(R"(/tag/(\d+))",UpdateTag);
    //获取标签信息
    //所有
    server.Get("/tag",GetAllTag);
    //单个
    server.Get(R"(/tag/(\d+))",GetOneTag);
    server.listen("192.168.164.128",2700);
    return 0;
}
