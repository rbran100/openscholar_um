/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

$(document).ready(function() {
    $('#sitelist').dataTable(
        {
        "bJQueryUI": false,    
        "bPaginate": true,
            "sPaginationType": "full_numbers"
        }
        );
} );


