<template>
  <f7-button big raised fill round color="red"
    @click="$emit('upload')">
    ▲ Nahrát ▲
  </f7-button>
</template>

<script>
export default {
  name: 'ButtonUpload',
}
</script>
