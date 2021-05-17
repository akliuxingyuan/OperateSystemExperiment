import Vue from 'vue'
import App from './App.vue'
import router from './router'
import "./assets/css/style.css";  //全局引入,将外部的css文件放到style文件下，只需在main.js文件中import
                                  //也可在组件中，局部引入，在<style>标签中，@import "./assets/css/style.css"; 

                                  
Vue.config.productionTip = false

new Vue({
  router,
  render: h => h(App)
}).$mount('#app')
