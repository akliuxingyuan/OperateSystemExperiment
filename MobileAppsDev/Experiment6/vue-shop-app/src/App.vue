<template>
  <div id="app">
    <!-- 路由展示 -->
    <router-view />
    <!-- 底部导航 -->
    <NavBottom />
  </div>
</template>

<script>
 import NavBottom from './components/NavBottom';

export default {
  components:{
    NavBottom
  }
}
</script>

<style>
@import './assets/css/index.css'
</style>
