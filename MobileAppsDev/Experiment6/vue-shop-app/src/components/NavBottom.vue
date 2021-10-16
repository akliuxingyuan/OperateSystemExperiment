<template>
  <div class="nav-bottom">
    <router-link to="/home">
      <i class="iconfont iconfenlei" ></i>
      <label>首页</label>
    </router-link>
    <router-link to="/find">
      <i class="iconfont iconweixin1" ></i>
      <label>发现</label>
    </router-link>
    <router-link to="/order">
      <i class="iconfont iconfenlei" ></i>
      <label>订单</label>
    </router-link>
    <router-link to="/my">
      <i class="iconfont iconfenlei" ></i>
      <label>我的</label>
    </router-link>
  </div>
</template>

<script>
export default {
  name: 'NavBottom',
  
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
.active {
  color:red;
}
</style>
