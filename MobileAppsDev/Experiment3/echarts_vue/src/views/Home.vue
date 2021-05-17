<template>
  <div class="home">
    <covid19 msg="Covid-19"/>
  </div>
</template>

<script>

import covid19 from '@/components/covid19.vue'

export default {
  name: 'Home',
  components: {
    covid19
  }
}
</script>
