<template>

  <ListOfAnimations
  :maxAnimations="6"
  :animations="animations"
  @select="debug(arguments[0]); uploadSelectedAnimation(arguments[0]);"/>

</template>

<script>
import axios from 'axios'
import { ESP_BASE_URL, axiosConfig } from '@/constants.js'

import ListOfAnimations from '@/components/ListOfAnimations.vue'

export default {
  name: 'SelectAnimationPage',
  components: {
	 ListOfAnimations
  },
  data: function () {
    return {
      animations: [
        { id: 0, name: 'Diagonalní vyplnění' },
        { id: 1, name: 'Diagonalní pruh' },
        { id: 2, name: 'Rostoucí čtverec vyplnění' },
        { id: 3, name: 'Rostoucí čtverec' },
        { id: 4, name: 'Bodové vyplnění' },
        { id: 5, name: 'Panáček' },
        { id: 6, name: 'Posouvající se čtverec' },
        { id: 7, name: 'Točící se čára' },
        { id: 8, name: 'Točící se vyplnění' },
        { id: 9, name: 'Linka s pulzem' },
      ],
    };
  },
  methods: {
  	debug (value) {
  		console.log('>', value)
  	},
    uploadSelectedAnimation (values) {
      this.$f7.dialog.preloader('Odesílá se nastavení...')

      let {duration, typeAnimation, selected} = values

      return axios.get(
        ESP_BASE_URL + '/load/',
        // 'http://httpbin.org/get'+'/text/',
        // 'http://httpbin.org/get',
        {
          timeout: 5 * 1000,
          params: {
            style: typeAnimation,
            d: duration,
            n: selected,
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
