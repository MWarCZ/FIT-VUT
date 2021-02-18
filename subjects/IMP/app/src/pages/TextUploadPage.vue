<template>

  <TextAnimation
  @upload="debug(arguments[0]); uploadTextAnimation(arguments[0]);"/>
</template>

<script>
import axios from 'axios'
import { ESP_BASE_URL, axiosConfig } from '@/constants.js'

import TextAnimation from '@/components/TextAnimation.vue'

export default {
  name: 'TextUploadPage',
  components: {
    TextAnimation
  },
  methods: {
  	debug (value) {
  		console.log('>', value)
  	},
    uploadTextAnimation (values) {
      this.$f7.dialog.preloader('Odesílá se nastavení...')

      let {duration, typeAnimation, text} = values
      console.log(`TAD(${text}, ${duration}, ${typeAnimation})`)
      return axios.get(
        ESP_BASE_URL + '/text/',
        // 'http://httpbin.org/get'+'/text/',
        // 'http://httpbin.org/get',
        {
          timeout: 5 * 1000,
          params: {
            style: typeAnimation,
            d: duration,
            t: text,
          },
          crossdomain: true,
          // ...axiosConfig,
        })
        .then(res => {
          this.$f7.dialog.close()
          console.log('OK:', res)
        })
        .catch(e => {
          this.$f7.dialog.close()

          if (e.response) {
            this.$f7.dialog.alert('Server nebyl schopny zpracovat pozadavek.', 'Chyba!')
          } else if (e.request) {
            this.$f7.dialog.alert('Server neodpovídá.', 'Chyba!')
          } else {
            this.$f7.dialog.alert('Nastala nějaká chyba.', 'Chyba!')
          }

          console.log('ERR:', e.request)
        })
    }
  },
}
</script>
