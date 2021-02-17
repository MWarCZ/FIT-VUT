<template>
  <f7-page>
    <f7-navbar>
      <f7-nav-left back-link="Back"></f7-nav-left>
      <f7-nav-title>LED Matice</f7-nav-title>
      <f7-nav-right>
        <ButtonUpload @upload="uploadScreen()"/>
      </f7-nav-right>
    </f7-navbar>

    <f7-block strong class="text-align-center" style="margin: 0;">

      <Display
        :matrix=matrix
        @click="debug(arguments[0]); refreshScreen(arguments[0])"
      />
      <br />
      <ButtonUpload @upload="uploadScreen()"/>
      <br />
      <f7-button big raised fill round color="blue"
        @click="cleanMatrix();">
        Vyčistit
      </f7-button>
    </f7-block>

  </f7-page>

</template>

<script>
import axios from 'axios'
import { ESP_BASE_URL, axiosConfig } from '@/constants.js'

import Display from '@/components/Display.vue'
import ButtonUpload from '@/components/ButtonUpload.vue'

export default {
  name: 'MatrixDisplay',
  components: {
	 Display,
   ButtonUpload,
  },
  data: function () {
    return {
      matrix: Array(8).fill(false).map(x => Array(8).fill(false)),
    }
  },
  methods: {
  	debug (value) {
  		console.log('>', value)
  	},
    refreshArray (arr) {
      arr.push(1)
      arr.pop()
    },
    row2num(row = []) {
      let num = 0
      row.forEach(c=>{
        num <<= 1
        num |= (c)?1:0
      })
      return num
    },
    refreshScreen (values) {
      let {row, col, value} = values
      this.matrix[row][col] =! value
      this.refreshArray(this.matrix)
    },
    cleanMatrix () {
      this.matrix = this.matrix.map(r=>r.map(()=>false))
    },
    uploadScreen () {
      this.$f7.dialog.preloader('Odesílá se nastavení...')

      // let {row, col, value} = values

      // this.matrix[row][col] =! value

      return axios.get(
        ESP_BASE_URL + '/set/',
        // 'http://httpbin.org/get'+'/text/',
        // 'http://httpbin.org/get',
        {
          timeout: 5 * 1000,
          params: {
            s: 0,
            l: 1,
            d: 50,
            r0: this.row2num(this.matrix[0]),
            r1: this.row2num(this.matrix[1]),
            r2: this.row2num(this.matrix[2]),
            r3: this.row2num(this.matrix[3]),
            r4: this.row2num(this.matrix[4]),
            r5: this.row2num(this.matrix[5]),
            r6: this.row2num(this.matrix[6]),
            r7: this.row2num(this.matrix[7]),
            style: 0,
          },
          crossdomain: true,
          // ...axiosConfig,
        })
        .then(res => {
          this.$f7.dialog.close()
          // this.refreshArray(this.matrix)
          console.log('OK:', res)
        })
        .catch(e => {
          this.$f7.dialog.close()
          // this.matrix[row][col] = value
          // this.refreshArray(this.matrix)

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
