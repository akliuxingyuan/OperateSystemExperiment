<template>
  <div class="home">
    详情页 {{id}}
    <a @click="goBack()">返回</a>
  </div>
</template>

<script>

export default {
  name: 'Detail',
  data(){
    return {
      id:0
    }
  },
  mounted:function(){
    this.id = this.$route.params.id;
  },
  methods:{
    goBack(){
      this.$router.push('/home')
    }
  }
}
</script>
