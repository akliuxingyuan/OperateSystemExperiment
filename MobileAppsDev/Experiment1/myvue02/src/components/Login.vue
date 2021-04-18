<template>
  <div class="login-box">
    <p class="username">用户名：<input type="text" v-model="userName" /></p>
    <p class="userpass">密码：<input type="text" v-model="passWord" /></p>
    <button @click="login">登录</button>
    <div v-show="infovisible">
      <p>{{ msg }}</p>
    </div>
  </div>
</template>

<script>
// import axios from 'axios'

export default {
  name: "login",
  data() {
    return {
      userName: "",
      passWord: "",
      infovisible: false,
      msg: "用户名密码错误",
    };
  },
  methods: {
    login() {
      if (this.userName === "root" && this.passWord === "root") {
        console.log("登录成功");
        // 页面进行跳转
        this.msg = "登录成功";
      }
      this.infovisible = true;
    },
  },
};
</script>

<style scoped>
.login-box {
  display: flex;
  flex-direction: column;
  align-items: center;
}

.username {
  display: flex;
  flex-direction: row;
  justify-content: right;
  width: 300px;
}

.userpass {
  display: flex;
  flex-direction: row;
  justify-content: right;
  width: 300px;
}
</style>