<template>
  <div class="home">
    <!-- 导航模块 -->
    <div class="index-category ">
      <div class="category" v-for="(list,i) in lists" :key="i">
        <i class="iconfont" :class="list.icon" :style="{background:list.color}"></i>
        <label>{{list.title}}</label>
      </div>
    </div>
    <!-- 轮播模块 -->
    <Banner  :imgArr="imgArr" :obj="{id:1,name:'abc'}"/>
    <!-- 列表展示模块 -->
    <div class="index-main">
      <ul>
        <li class="lists" v-for="item in items" :key="item.product_id">
          <router-link :to="'/detail/'+item.product_id">
            <img :src="item.product_img_url" width="150" height="150"/>
          </router-link>
          <label>
            <b class="discount">{{item.product_uprice}}</b>
            <span class="price-text">{{item.product_price}}</span>
          </label>
        </li>
      </ul>
    </div>
  </div>
</template>

<script>
 import Banner from '../components/Banner';

export default {
  name: 'Home',
  data(){
    return {
      timer:null,
      n:0,
      lists:[
        {title:'美食1',icon:'icontongxunlu',color:'#f60'},
        {title:'美食2',icon:'iconfaxian',color:'#ecbe35'},
        {title:'美食3',icon:'icontongxunlu',color:'#92d85'},
        {title:'美食4',icon:'iconshouye1',color:'#f60'},
        {title:'美食5',icon:'iconfenlei',color:'#f60'},
        {title:'美食5',icon:'iconfenlei',color:'#f60'},
        {title:'美食5',icon:'iconfenlei',color:'#f60'},
        {title:'美食5',icon:'iconfenlei',color:'#f60'}
      ],
      imgArr:[
        require('../assets/img/3.jpg'),
        require('../assets/img/2.jpg'),
        require('../assets/img/3.jpg')
      ],
      items:[]
    }
  },
  components:{
    Banner
  },
  methods:{
    getList(){
      this.$http.get('/home/page/3/20').then(res=>{
        console.log(res);
        this.items = res.data.data;
      })
    }
  },
  mounted:function(){
    this.getList();
  }
}
</script>
