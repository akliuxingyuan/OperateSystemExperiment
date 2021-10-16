<template>
  <div class="banner"> 
      <img alt="Vue logo" v-for="(v,i) in imgArr" :key="i" :src="v" v-show="n==i">
      <div class="banner-circle">
        <ul>
          <li v-for="(v,i) in imgArr" :key="i" :class="n==i?'selected':''"></li>
        </ul>
      </div>
    </div>
</template>

<script>
export default {
  name: 'Banner',
  data(){
    return {
      timer:null,
      n:0,
    }
  },
  //props:["imgArr"],  //数组定义方式
  props:{
    imgArr:{

    },
    obj:{
      id:Number,
      name:String
    }
  },
  methods:{
    play(){
      this.timer = setInterval(this.autoPlay,2000)
    },
    autoPlay(){
      this.n++;
      if(this.n == this.imgArr.length){
        this.n = 0;
      }
    }
  },
  mounted:function(){  //挂载完成
    this.play();
  },
  destroyed:function(){  //销毁
    clearInterval(this.timer)
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>

</style>
