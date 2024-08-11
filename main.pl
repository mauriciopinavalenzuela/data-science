use strict;
use warnings;

my %carrito;

sub mostrar_menu {
    print "\nMenú de Compras:\n";
    print "1. Agregar producto al carrito\n";
    print "2. Ver carrito de compras\n";
    print "3. Eliminar producto del carrito\n";
    print "4. Salir\n";
    print "Selecciona una opción: ";
}

sub leer_entrada {
    chomp(my $entrada = <STDIN>);
    return $entrada;
}

sub agregar_producto {
    print "Nombre del producto: ";
    my $producto = leer_entrada();
    
    print "Cantidad: ";
    my $cantidad = leer_entrada();
    
    $carrito{$producto} += $cantidad;
    print "Producto agregado al carrito.\n";
}

sub ver_carrito {
    if (%carrito) {
        print "\nCarrito de Compras:\n";
        foreach my $producto (keys %carrito) {
            print "$producto: $carrito{$producto}\n";
        }
    } else {
        print "El carrito está vacío.\n";
    }
}

sub eliminar_producto {
    print "Nombre del producto a eliminar: ";
    my $producto = leer_entrada();
    
    if (exists $carrito{$producto}) {
        delete $carrito{$producto};
        print "Producto eliminado del carrito.\n";
    } else {
        print "El producto no se encuentra en el carrito.\n";
    }
}

sub main {
    while (1) {
        mostrar_menu();
        my $opcion = leer_entrada();
        
        if ($opcion == 1) {
            agregar_producto();
        } elsif ($opcion == 2) {
            ver_carrito();
        } elsif ($opcion == 3) {
            eliminar_producto();
        } elsif ($opcion == 4) {
            print "Saliendo del programa.\n";
            last;
        } else {
            print "Opción no válida. Por favor, selecciona una opción del 1 al 4.\n";
        }
    }
}


main();
