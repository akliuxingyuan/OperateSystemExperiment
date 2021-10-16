<template>
  <div class="home">
    发现页展示
    <Banner :imgArr="abc"/>
  </div>
</template>

<script>
import Banner from '../components/Banner';

export default {
  name: 'Find',
  data(){
    return {
      abc:[
        require('../assets/img/4.jpg'),
        require('../assets/img/5.jpg'),
        require('../assets/img/6.jpg')
      ]
    }
  },
  components:{
    Banner
  }
}
</script>
