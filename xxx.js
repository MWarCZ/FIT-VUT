const MODE = {
  SELECT: 'select',
  ADD: 'add',
  REMOVE: 'remove',
  ARROW: 'arrow',
  TEXT: 'text',
  MOVE: 'move',
}

const App = function (idOfDrawingDiv, width=1000, height=1000 ) {
  // Hlavni kreslici plocha
  this.draw = SVG(idOfDrawingDiv).size(width, height).attr({style:'background:#fff'})

  // TODO
  this.drawContainer = this.draw.group()
  this.drawMarkers  = this.draw.group()
  // Obekt s poli
  this.listOf = {
    // List vsech propoju mezi elementy diagramu
    conns: [],
    // List vsech elementu diagramu
    dElements: []
  }

  // Aktualni mod
  this.nowMode = MODE.SELECT
  // Predchozi mod
  this.oldMode = MODE.SELECT

  // Prave vybrany element diagramu
  this.selectedElement = undefined

  // Novy element pro pridani do diagramu
  this.newElement = undefined

  // Kreslici plocha => Akce
  this.draw.on('click', e=>{
    console.log('draw click')
    switch(this.nowMode) {
      case MODE.ADD:
        this.createNewDElement(e, this.draw, DElementFactory.Rect);
        this.changeMode(this.oldMode)
        break;
    }
  })
  this.draw.on('mouseup', e=>{
    console.log('draw mouseup')
    switch(this.nowMode) {
      case MODE.SELECT:
        if(!!this.selectedElement) {
          this.selectElement(undefined)
        }
        break;
    }
  })

} // const App
App.prototype = {
  changeMode: function (newMode=MODE.SELECT) {
    this.oldMode = this.nowMode
    this.nowMode = newMode
    console.log('Change MODE:')
    console.log(' > New MODE:', this.nowMode)
    console.log(' > Old MODE:', this.oldMode)
  }, // changeMode

  selectElement: function (element) {
    // Pred zmenou elementu
    if(!!this.selectedElement) {
      this.selectedElement.select(false)
      //this.selectedElement.group.draggy(true)
    }

    // Zmena elementu
    this.selectedElement = element

    // Po zmene elementu
    if(!!this.selectedElement) {
      this.selectedElement.select(true)
      //this.selectedElement.group.draggy()
    }

    // console.log('Change selected element:', this.selectedElement)
    console.log('Change selectedElement.')
  }, // selectElement

  changeNewElement: function (dElement) {
    this.newElement = dElement
    console.log('Change newElement.')
  }, // changeNewElement

  createConnection: function(dElement1, dElement2, click_callback) {
    // Vytvoreni propoje mezi elementy 1 a 2
    let conn1 = dElement1.group.connectable({
      container: this.drawContainer,
      markers: this.drawMarkers,
      marker: 'default',
      targetAttach: 'perifery',
      sourceAttach: 'perifery',
      color: '#000',
    }, dElement2.group)

    // Vizualni uprava sipky
    conn1.marker.attr({
      markerWidth: '5',
      markerHeight: '5',
      refX: '28',
    })
    conn1.connector.attr({'stroke-width':5})

    // Nastaveni callbacku pro kliknuti
    if(!!click_callback) {
      conn1.connector.on('click', click_callback);
    }

    // Prida propoj do globalniho listu propoju.
    this.listOf.conns.push(conn1)
    return conn1;
  }, // createConnection

  removeConnection: function(conn) {
    this.listOf.conns = this.listOf.conns.filter(con => con !== conn)
    conn.marker.remove()
    conn.connector.remove()
  }, // RemoveConnection

  removeConnectionsByDElement(dElement) {
    this.listOf.conns = this.listOf.conns.filter(conn => {
      // Pokud cil nebo zdroj propoje je dElement, tak smazat
      if( conn.source === dElement.group ||
        conn.target === dElement.group
      ) {
        conn.marker.remove()
        conn.connector.remove()
        return false;
      } else {
        return true;
      }
    })
  }, // removeConnectionsByDElement

  createNewDElement: function (e, draw, factoryFunction = DElementFactory.Rect) {
    console.log('DElementManager create: ', e)
    let dElement = factoryFunction(draw)
    dElement.group.move(e.offsetX, e.offsetY)

    // Akce pro element na plose
    dElement.group.on('click', e => {
      console.log('dElement click')
      switch(this.nowMode) {
        case MODE.SELECT:
          this.selectElement(dElement)
          break;

        case MODE.REMOVE:
          this.selectElement(undefined)
          // Smazani sipek mezi elementy
          this.removeConnectionsByDElement(dElement)
          // smazani elementu z obrazku
          dElement.remove()
          // uvolneni elementu
          delete dElement
          break;

        case MODE.ARROW:
          if (!!this.selectedElement) {
            // Pokud jsou vzbrane elementy rozdilne
            if (this.selectedElement !== dElement) {
              // Vytvoreni propoje/sipky
              let conn = this.createConnection(this.selectedElement, dElement)
              // Akce pro propoj
              conn.connector.on('click', (e)=>{
                console.log('conn click')
                if(this.nowMode === MODE.REMOVE) {
                  this.removeConnection(conn);
                }
              })
              conn.connector.on('mouseenter', e => {
                console.log('conn mouseenter')
                conn.setConnectorColor('#f00')
              })
              conn.connector.on('mouseleave', e => {
                console.log('conn mouseleave')
                conn.setConnectorColor('#000')
              })
            }

            // Uz je spojeni => odebere se vyber
            this.selectElement(undefined)
          } else {
            // Vybere se prvni element se kterim se spojuje
            this.selectElement(dElement)
          }
          break;
      } // switch
    }) // click
    dElement.group.on('mousedown', (e)=>{
      console.log('dElement mousedown')
      switch(this.nowMode) {
        case MODE.SELECT:
          this.selectElement(dElement)
          break;
      }
    }) // mousedown
    dElement.group.on('mouseup', (e)=>{
      console.log('dElement mouseup')
      switch(this.nowMode) {
        case MODE.MOVE:
          //changeMode(MODE.SELECT);
          break;
      }
    }) // mouseup
    dElement.group.on('mouseenter', e => {
      switch(this.nowMode) {
        case MODE.SELECT:
          dElement.group.draggy()
          break;
      }
      console.log('dElement mouseenter')
      dElement.focus(true)
    }) // mouseenter
    dElement.group.on('mouseleave', e => {
      console.log('dElement mouseleave')
      dElement.group.draggy(true)
      dElement.focus(false)
    }) // mouseleave


    // TODO upravit nejak elegantne resize
    // Resize element
    let isMoved = false;
    dElement.resizer.on('mouseenter', (e)=>{
      console.log('resizer mouseenter')
      if(!!this.selectedElement) {
        this.selectedElement.group.draggy(true)
      }
    });
    dElement.resizer.on('mouseleave', (e)=>{
      console.log('resizer mouseleave')
      if(!!this.selectedElement) {
        this.selectedElement.group.draggy()
      }
    });
    dElement.resizer.on('mousedown', (e)=>{
      console.log('resizer mousedown')
      isMoved = true;
    });
    dElement.resizer.on('mouseup', (e)=>{
      console.log('resizer mouseup')
      isMoved = false;
      this.listOf.conns.forEach(con => {
        con.update()
      })
    });
    this.draw.on('mousemove', (e)=>{
      console.log('draw mousemove')
      switch(this.nowMode) {
        case MODE.SELECT:
          if(isMoved && !!this.selectedElement) {
            let x = e.offsetX - this.selectedElement.group.x()+5
            let y = e.offsetY - this.selectedElement.group.y()+5
            this.selectedElement.resize(x, y);
            this.listOf.conns.forEach(con => {
              con.update()
            })
          }
          break;
      } // switch
    });


  }, // createNewDElement

} // App.prototype


/* */
const DElement = function (svgGroup, svgItems = []) {
  this.group = svgGroup
  this.items = svgItems
  this.resizer = undefined
  this.selected = false
  this.focused = false
} // const DElement
DElement.prototype = {
  resize () { console.log('DElement default resize.') },
  remove () { console.log('DElement default remove.') },
  select () { console.log('DElement default select.') },
} // DElement.prototype


const DElementFactory = {
  // svg: new SVG('elementgroup0_box').size(1, 1),

  Rect (draw) {
    let size = [150, 100];
    let group = draw.group();
    // Vysledny DElement
    let dElement = new DElement(group)

    let outline = group.rect(size[0], size[1]).attr({
      fill: 'transparent',
      stroke: '#00f',
      'stroke-width': 0,
      'stroke-dasharray': "20,0"
    });

    let rec = group.rect(size[0], size[1]).attr({
      fill: '#ffff00',
    });

    let txt = group.text("Nějaký\ntext.").move(10,10);

    let resize = group.polygon('20,0 0,20 20,20').attr({
      fill: '#00f',
      opacity: 0,
    });
    resize.addClass('resize-nw')
    resize.x( size[0]-resize.width() );
    resize.y( size[1]-resize.height() );

    // Vracena struktura elementu
    dElement.items = [ rec, outline, txt, resize ]
    dElement.resizer = resize

    /**
     * Funkce elementu pro zmenu velikosti.
     */
    dElement.resize = function (w, h) {
      if (w<0 || h<0)
        return
      rec.size(w,h);
      outline.size(w,h);
      resize.x( w-resize.width() );
      resize.y( h-resize.height() );
    }
    /**
     * Funkce elementu pro zmenu velikosti.
     */
    dElement.remove = function () {
      group.remove()
    }
    /**
     * Funkce elementu pro vizualni znazorneni vyberu elementu.
     */
    let refreshOutline = function () {
      if(dElement.selected) {
        outline.attr({
          'stroke-width': 5,
          stroke: '#00f',
        });
        resize.attr({
          opacity: 1,
        });
      } else if(dElement.focused) {
        outline.attr({
          'stroke-width': 5,
          stroke: '#f00',
        });
      } else {
        outline.attr({
          'stroke-width': 0,
          stroke: '#000',
        });
        resize.attr({
          opacity: 0,
        });
      }
    }
    dElement.select = function (selectOrUnselect = true) {
      dElement.selected = selectOrUnselect
      refreshOutline()
    }
    dElement.focus = function (focusOrUnfocus = true) {
      dElement.focused = focusOrUnfocus
      refreshOutline()
    }

    return dElement
  }, // Rect

} // const DElementFactory


