<template>
  <table>
    <tr v-for="(values, r) in matrix">
      <td v-for="(value, c) in values">
        <b class="label" @click="emitClick(r,c)">
          <!-- <input  type="checkbox" v-model="matrix[r-1]"
                  :value="c-1"
                  :click="$emit('input', matrix)"
                  /> -->
          <input  type="checkbox"
            :checked="matrix[r][c]"
            />
          <span></span>
        </b>
      </td>
    </tr>
  </table>
</template>

<script>

export default {
  name: 'Display',
  props: {
    matrix: {
      default: ()=>{
        let matrix //= []
        // for(let i=0; i<8; i++) { matrix.push([]); }
        matrix = Array(8).fill(false).map(x => Array(8).fill(false))
        return matrix
      }
    }
  },
  data: function () {
    return {}
  },
  methods: {
    emitClick (row, col) {
      console.log('M: ', this.matrix )
      this.$emit('click', {
        row: row,
        col: col,
        value: this.matrix[row][col],
      })
    }
  },
}
</script>

<style scoped>
  table {
    width:100%;
    height:100vw;
    /* padding: 5vw; */
  }
  input[type=checkbox] {
    display: none;
  }
  .label {
    display: block;
    height: 100%;
    width: 100%;
  }
  .label span {
    display: block;
    height: 100%;
    width: 100%;
    background-color: silver;
  }
  input[type=checkbox]:checked + span {
    background-color: red;
  }
</style>
