<?php

/**
 * @function vsite_site_register_form
 *
 * Provides form for site/register
 */
function vsite_register_form($form, &$state) {
    global $base_url;
    global $user;
    $vsites = vsite_get_vsite_by_owner($user->uid);
    if(count($vsites) > 1){
      
        drupal_set_message("You have already registered a site.", "error");
        drupal_goto("<front>");
    }
    //$user->name is the user's UCM network ID
    $form = array();
    $anonymous = ($user->uid == 0);
    $manager = (user_access('create vsite content on behalf') && user_access('administer users'));

    $parent = null;
    if (isset($_GET['parent']) && $parent = vsite_get_vsite($_GET['parent'])) {
        $parent = $_GET['parent'];
    }

    // clear any drupal_messages. Ajax takes care of them
    $form['#prefix'] = '<div id = "user_register">';
    $form['#suffix'] = '</div>';

    $form['site_type'] = array(
        '#type' => 'radios',
        '#name' => 'site_type',
        '#title' => t('Type of Site'),
        '#description' => t('Choose whether you would like to use our site building software or build your own site from scratch.'),
        '#default_value' => 0,
        '#options' => array(
            0 => t('I want to use the tools available through the OpenScholar site builder and have the UC Merced brand'),
            1 => t('I will build my own site on my local machine and just upload it')
        ),
        '#prefix' => '<div id="site-type-item">',
        '#suffix' => '</div>',
    );

    $form['domain'] = array(
        '#title' => 'Domain name',
        //'#attributes' => array('readonly' => 'readonly'),
       '#type' => 'textfield',
        '#default_value' => $user->name,
        '#states' => array(
      'invisible' => array(
       ':input[name="site_type"]' => array('value' => 1),
      ),
    ),
    );
    
    $form['title'] = array(
        '#title' => 'Site title',
        '#type' => 'textfield',
        '#default_value' => $user->name,
    );
    
    
    $form['description'] = array(
        '#title' => 'Site description',
        '#type' => 'textfield',
        '#default_value' => $user->name . "'s Website",
    );

    $form['mail'] = array(
        '#type' => 'hidden',
        '#value' => $user->name . "ucmerced.edu",
    );

    $form['name'] = array(
        '#type' => 'hidden',
        '#value' => $user->name,
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Create your site'),
        '#weight' => 40,
        '#prefix' => '<div id="form-errors"></div>',
        '#suffix' => '<div id="submit-suffix"></div>',
    );

    return $form;
}

//returns user section of vsite_register form
function _vsite_register_user_form() {

    $form = array();
    $form['name'] = array(
        '#type' => 'textfield',
        '#size' => 35,
        '#title' => t('Desired user name'),
        '#description' => t('Spaces are allowed; punctuation is not allowed except for periods, hyphens, and underscores.'),
        '#ajax' => array(
            'callback' => 'vsite_register_ajax_validation',
            'wrapper' => 'name-errors',
        ),
        '#field_suffix' => theme('vsite_register_form_errors', array('name' => 'name')),
    );

    $form['first_name'] = array(
        '#type' => 'textfield',
        '#size' => 35,
        '#title' => t('First name'),
    );

    $form['last_name'] = array(
        '#type' => 'textfield',
        '#size' => 35,
        '#title' => t('Last name'),
    );

    $form['mail'] = array(
        '#type' => 'textfield',
        '#size' => 35,
        '#title' => t('Email address'),
        '#description' => t('A valid e-mail address is required. All e-mails from OpenScholar will be sent to this address.'),
        '#ajax' => array(
            'callback' => 'vsite_register_ajax_validation',
            'wrapper' => 'mail-errors',
        ),
        '#field_suffix' => theme('vsite_register_form_errors', array('name' => 'mail')),
    );

    // password field
    $form['password'] = array(
        '#type' => 'password_confirm',
        '#size' => 35,
    );
    /*
      $form['notify'] = array(
      '#type' => 'checkbox',
      '#title' => t('Send email notification of this account.'),
      );
     */
    return $form;
}

/**
 * @function vsite_site_register_form
 *
 * Creates user and vsite when site/register form is submitted
 */
function vsite_register_form_submit(&$form, &$state) {
    ctools_include('user', 'os');
    ctools_include('vsite', 'vsite');
    $values = $state['values'];
//die(var_dump($values));
//  if ($values['vicarious_user'] && $values['existing_username']) {
//    //user account already exists
//    $site_owner = user_load_by_name($values['existing_username']);
//
//  } elseif (($values['vicarious_user'] && !$values['existing_username']) || (!$values['vicarious_user'] && $values['name'])) {
//    // create user for self or on someone's behalf
//    $user_options = array(
//    	'name' => $values['name'],
//    	'pass' => $values['password'],
//    	'mail' => $values['mail'],
//    	'status' => 1,
//      'field_first_name' => $values['first_name'],
//      'field_last_name' => $values['last_name'],
//    );
//    $site_owner = os_user_create($user_options);
//
//    // Send out an email to notify the user of the newly created account.
//    // Email only sent if user requested to be notified of new account.
//    // if ($values['notify']) {
//    //  _user_mail_notify('register_no_approval_required', $site_owner);
//    // }
//
//    //log in as new user, if we're not already logged in.
//    global $user;
//    if ($user->uid == 0) {
//      $user = $site_owner;
//      user_login_finalize($state);
//    }
//
//  } else {
    //creating site for self.  no need to create a new user
    global $user;
    $site_owner = $user;

//  }
    //set the domain name which will either be the username or value from form
    if($values['domain'] == ''){
        $domain = $user->name;
    }else{
        $domain = $values['domain'];
        $domain = $values['domain'];
    }
//
    //if the type chosen is "FTP", then create an SSH site
    if ($values['site_type'] == '1') {
        $domain = $user->name;
        //check to see if web folder already exists
        if (file_exists('/var/www/html/faculty_sites/' . $domain)){
            drupal_set_message("That domain already exists; please try again.", 'error');
            drupal_goto('site/register');
        }
        //Create alias file in /var/www/html/httpd_conf/aliases
        //This will point http://<sitename>/<domainname> to the user's`1
        //web folder
        $alias_file = '/var/www/html/httpd_conf/aliases/' . $domain;
        $data = <<<EOF
Alias /$domain /var/www/html/faculty_sites/$domain
EOF;
        $handle = fopen($alias_file, 'w');
        fwrite($handle, $data);
        fclose($handle);
        //create web directory in /var/www/html/faculty_sites
        $result = mkdir('/var/www/html/faculty_sites/' . $domain);

        if ($result) {
            //make the user the owner of the web folder
            chown('/var/www/html/faculty_sites/' . $domain, $user->name);
            $data = <<<EOF
           <html>
               <head>
                 <title>Faculty Site of $user->name</title>
               </head>
                <body>
                Welcome to your new site. 
                </body>
          </html>
EOF;
            $handle = fopen('/var/www/html/faculty_sites/' . $domain . '/index.html', 'w');
            fwrite($handle, $data);
            fclose($handle);
            drupal_set_message("Created home folder");
            
            //GRAB USER INFO FROM LDAP AND USE IT TO POPULATE
            //SITE_INFO NODE
            create_siteinfo_node($user->name, $domain);
            
            
//            $node = new stdClass();
//            $node->type = 'site_info';
//            node_object_prepare($node);
//            $node->title = $domain;
//            $node->field_site_name[LANGUAGE_NONE]['0']['url'] = $domain;
//            $node->field_site_name[LANGUAGE_NONE]['0']['title'] = $values['title'];
//            $node->field_ucm_site_description[LANGUAGE_NONE]['0']['value'] = $values['description'];
//            node_save($node);
            
        } else {
            drupal_set_message("Failed to create home folder.", 'error');
            drupal_goto('site/regsiter');
        }
        //db_query("INSERT INTO {vsite_managed} (`name`, `status`, `process`) VALUES ('%s', %d, %d)", $domain, 1, 0);
        //set file flag that indicates an apache restart is necessary.
        touch('/var/www/html/RESTART_APACHE');
        drupal_set_message("Site created! Within five minutes it will be available at " . $baseurl . "/" . $domain);
        drupal_goto($base_url);
    } else {
        //they've chosen to create an openscholar site
        if ($vsite = vsite_create_vsite($values['title'], $domain, $site_owner->uid)) {
            //die(print_r($site_owner));
            //die('<pre>' . print_r($vsite,1) . '</pre>');
            //dpm($vsite->group->nid);
            
            create_siteinfo_node($user->name, $domain);
            
//            $node = new stdClass();
//            $node->type = 'site_info';
//            node_object_prepare($node);
//            $node->title = $values['title'];
//            $node->field_nid[LANGUAGE_NONE]['0']['value'] = $vsite->group->nid;
//            $node->field_site_name[LANGUAGE_NONE]['0']['url'] = $domain;
//            $node->field_site_name[LANGUAGE_NONE]['0']['title'] = $values['title'];
//            $node->field_ucm_site_description[LANGUAGE_NONE]['0']['value'] = $values['description'];
//            node_save($node);
            //IF THE USER HAS A NON-OS SITE, DELETE THE APACHE ALIAS DIRECTIVE TO THE OLD SITE
            if(file_exists('/var/www/html/httpd_conf/aliases/' . $domain)){
                unlink('/var/www/html/httpd_conf/aliases/' . $domain);
                touch('/var/www/html/RESTART_APACHE');
                drupal_set_message("Site created! Within five minutes it will be available at faculty.ucmerced.edu/" . $domain);
                drupal_goto($base_url);
            }
           // die(print_r('<pre>' . $node . '</pre>', 1));        
            //db_query("INSERT INTO {vsite_managed} (`name`, `status`, `process`) VALUES ('%s', %d, %d)", $values['domain'], 0, 0);
            //add user to vsite
            module_load_include('inc', 'vsite', 'includes/user');
            vsite_user_add($site_owner);

            os_role_grant($site_owner->uid, 'faculty', $vsite->id);

            //$message = vsite_register_message($form, $values['domain']);
            $message = "Site created!";
            $commands[] = ajax_command_replace('#submit-suffix', $message);
            $commands[] = ajax_command_remove('#edit-submit');

            if (isset($values['vsite_private']) && $values['vsite_private'] && module_exists('vsite_access')) {
                $node = node_load($vsite->group->nid);
                $node->group_access[LANGUAGE_NONE][0]['value'] = 1;
                node_save($node);
            }

            // Email user to inform them of new created site, email is always sent.
            vsite_register_notify_user($vsite);

            //it worked, clear the errors
            drupal_get_messages('error');
        } else {

            $commands[] = _vsite_register_form_error();
        }

        //call any other submit functions here
        foreach ($form['#submit'] as $fnc) {
            if (__FUNCTION__ !== $fnc) {
                call_user_func($fnc, $form, $state);
            }
        }
        //print ajax_render($commands);
        drupal_set_message("Congratulations! Your site has been created.");
        drupal_goto($base_url . $domain);
        //print ajax_render("Site created!");
        exit;
    }
}

/**
 * @function vsite_site_register_form_validate
 *
 * Validation function for site/register form
 */
function vsite_register_form_validate(&$form, &$state) {
    //die(var_dump($state['values']['domain']));
//Ajax validation.
    if ($_GET['q'] == 'system/ajax') {
        if ($state['triggering_element']['#ajax']['callback'] == 'vsite_register_ajax_validation' ||
                $state['triggering_element']['#ajax']['callback'] == 'iqss_pinserver_register_ajax_validation') {
            //ajax callback will validate just this element and return a message.
            return;
        } else if ($state['triggering_element']['#ajax']['callback'] != 'vsite_register_ajax_avalible_presets') {
            //form is done.  maybe this should happen after the validate?  or at least duplicate its efforts.
            return vsite_register_ajax_submit_validation($form, $state);
        }
    }

    //Fall back to standard validation
    $values = $state['values'];

    foreach (vsite_register_validate_elements($values) as $element => $msg) {
        if ($msg) {
            form_set_error($element, $msg);
        }
    }

    if ($values['create_new_user']) {
        //tell javascript to switch to new user form instead of making user switch it again.
        drupal_add_js(array('vsite_register' => array('new_user_clicked' => TRUE)), 'setting');
    }
}

/**
 * @function vsite_register_ajax_validation
 *
 * Callback function for ajax.  returns errors on ajaxy elements
 */
function vsite_register_ajax_validation(&$form, &$state) {

    drupal_get_messages(); //clear this to avoid having notices clutter the form during ajax validation
    $elem = $state['triggering_element']['#name'];
    return theme('vsite_register_form_errors', (array(
        'name' => $elem,
        'errors' => _vsite_register_validate($elem, $state['values']),
    )));
    //$errors = form_get_errors();//$state['triggering_element']['#name']);
}

/**
 * Update the avalible presets
 *
 * @param $form
 * @param $form_state
 * @return $form
 */
function vsite_register_ajax_avalible_presets($form, $form_state) {

    //Hard work is done in the form build function, just return the item
    return $form['preset'];
}

/**
 * @function vsite_register_ajax_submit_validation
 *
 * Called when submit button is clicked and ajax is enabled.
 */
function vsite_register_ajax_submit_validation(&$form, &$state) {

    $values = $state['values'];

    $valid = $commands = array();

    foreach (vsite_register_validate_elements($values) as $element => $msg) {
        if ($msg) {
            $html = theme('item_list', array('items' => array($msg), 'type' => 'ul', 'attributes' => array('class' => array('error'))));
            $commands[] = ajax_command_html("#${element}-errors", $html);
        } else {
            $valid[] = ajax_command_html("#${element}-errors", '');
        }
    }

    //print new errors
    if (count($commands)) {
        print ajax_render(array_merge($commands, $valid));
        exit;
    }
}

/**
 * @function vsite_register_validate_elements
 *
 * Validates form elements, returns errors.  Errors can be handled as usual or ajax rendered.
 *
 * Exists as separate function because there's more logic than usual in choosing which elements need to validate
 * based on the state of the form.
 *
 * @param array $values
 *   Form state's values array
 * @return array
 *   element => error
 */
function vsite_register_validate_elements($values) {

//always check domain and form
    $elements = array('domain', 'form');

    //new user
    if ($values['create_new_user']) {
        $elements = array_merge($elements, array('name', 'mail', 'password'));
    }

    //existing user
    if ($values['vicarious_user'] && !$values['create_new_user']) {
        $elements = array_merge($elements, array('existing_username'));
    }

    $msgs = array();
    foreach ($elements as $element) {
        if ($errors = _vsite_register_validate($element, $values)) {
            $msgs[$element] = implode(' ', $errors);
        }
    }

    return $msgs;
}

/**
 * @function _vsite_register_validate
 *
 * vsite_register_ajax_validation and vsite_register_form_validate are wrappers for this function.
 * Common elements are validated here.
 *
 * @param string $element
 * 	Name of element to validate
 *
 * @param array $values
 * 	values array from returned form state
 *
 * @return string listing all errors with form element.  Null if no errors.
 */
function _vsite_register_validate($element, &$values) {
//die("ELEMENT:<br/><pre>".print_r($element, 1)."</pre><br/><br/>VALUES:<br/><pre>".print_r($values,1)."</pre>");
    $msg = array();
    $invalid_names = array("user", "site", "users", "home", "node", "poop");
    switch ($element) {
   
        case 'domain':
            if (in_array($values['domain'], $invalid_names) || strlen($values['domain']) < 3 || !valid_url($values['domain']) || !_vsite_register_valid_url($values['domain'])) {
                $msg[] = t('Invalid URL specified.');
            }
            if (($purl = purl_load(array('value' => $values['domain'], 'provider' => 'spaces_og'), TRUE)) || menu_get_item($values['domain'])) {
                $msg[] = t('URL %url is taken.  Please choose another.', array('%url' => $values['domain']));
            }

            break;

        case 'mail':
            if ($mail_error = user_validate_mail($values['mail'])) {
                $msg[] = $mail_error;
            }

            if (_vsite_register_mail_in_use($values['mail'])) {
                $msg[] = t('Email address already in use.  Please try another.');
            }
            break;

        case 'name':
            if ($user_error = user_validate_name($values['name'])) {
                $msg[] = $user_error;
            }

            if ($user = user_load_by_name($values['name'])) {
                $msg[] = t('Username %name is taken.  Please choose another.', array('%name' => $values['name']));
            }
            break;

        //check that all dependent fields are provided.
        case 'form':
            //on behalf selected, but no user specified.
            if ($values['vicarious_user'] && !(strlen($values['existing_username']) || $values['create_new_user'])) {
                $msg[] = t('You have selected to create this site on behalf of another user.  Please specify an existing user or create a new account.');
            }

            //new user
            if ($values['create_new_user']) {
                if (!(strlen($values['name']) && strlen($values['mail']) && strlen($values['password']))) {
                    $msg[] = t('You have selected to create a new user.  Please provide a username, email address, and password for the new user.');
                }

                //confirm_password
                $pass1 = trim($_POST['password']['pass1']);
                $pass2 = trim($_POST['password']['pass2']);
                if (!empty($pass1) || !empty($pass2)) {
                    if (strcmp($pass1, $pass2)) {
                        $msg[] = t('The specified passwords do not match.');
                    }
                } else {
                    $msg[] = t('The password field is required.');
                }
            }

            //existing user
            if ($values['vicarious_user'] && !$values['create_new_user']) {
                if (!user_load_by_name($values['existing_username'])) {
                    $msg[] = t('User @name does not exist.  Please try another user name or create a new user account instead.', array('@name' => $values['existing_username']));
                }
            }
            break;
    }

    return (count($msg)) ? $msg : NULL;
}

/**
 * @function _vsite_mail_in_use
 *
 * Checks if an email address is already in use by another user.
 * Code borrowed from user.module's user_account_form_validate()
 *
 * @param string $mail
 * 	email address
 */
function _vsite_register_mail_in_use($mail) {
    return (bool) db_select('users')
                    ->fields('users', array('uid'))
                    ->condition('mail', db_like($mail, 'LIKE'))
                    ->range(0, 1)
                    ->execute()
                    ->fetchField();
}

/**
 * @function _vsite_register_valid_url
 *
 * Checks a url for bad characters.
 *
 * @param string $url
 */
function _vsite_register_valid_url($url) {
    foreach (array('!', ';', '/', '?', ':', '@', '&', '=', '"', "'",) as $char) {
        if (strpos($url, $char) !== FALSE) {
            return FALSE;
        }
    }

    return purl_validate(array('value' => $url, 'provider' => 'spaces_og'));
}

/*
 * Function to send out an email to the user
 * to notify them of their newly created site.
 */

function vsite_register_notify_user($vsite) {
    global $language;
    global $base_url;
    global $user;

    // Check to see if the user logged in matched the newly created user.
    if ($user->uid == $vsite->group->uid) {
        $need_pass_reset = FALSE;
    } else {
        $need_pass_reset = TRUE;
    }

    $params = array();
    $from = variable_get('site_mail', ini_get('sendmail_from'));
    $new_user = user_load($vsite->group->uid);

    //add vsite and user object
    $params['vsite'] = $vsite;
    $params['user'] = $new_user;

    $scholar_site_url = url($base_url . '/' . $vsite->group->title, $options = array('absolute' => TRUE));
    $count = count(vsite_get_vsite_by_owner($new_user->uid));
    $timestamp = time();

    $subject = t("An OpenScholar web site has been created for you.", array(
        '!site_name' => variable_get('site_name', 'OpenScholar')
    ));
    $msg_variables = array(
        '!vsite-count' => $count,
        '!username' => $new_user->name,
        '!site' => variable_get('site_name', 'OpenScholar'),
        '!scholar_site_url' => $scholar_site_url,
        '!login_url' => user_pass_reset_url($new_user),
        '!site_login_uri' => url($base_url . '/user', array('absolute' => TRUE)),
        '!uri' => $base_url,
        '!uri_brief' => preg_replace('!^https?://!', '', $base_url),
        '!mailto' => $new_user->mail,
        '!date' => format_date(time(), 'medium', '', NULL, $message['language']->language),
        '!login_uri' => url('user', array(
            'absolute' => TRUE,
            'language' => $message['language']
        )),
        '!site_url' => url($vsite->group->purl),
        '!edit_uri' => url('user/' . $new_user->uid . '/edit', array(
            'absolute' => TRUE,
            'language' => $message['language']
        )),
        '!scholar_edit_url' => url("/user/{$new_user->uid}/edit", array('absolute' => TRUE)),
    );

    //detemine if user is creating first site or additional site, send appropriate message
    if (count(vsite_get_vsite_by_owner($new_user->uid)) > 1 || $need_pass_reset == FALSE) {
        $body = t("Hi !username,\n\nYour new web site at !site has been activated. Your web site url is !scholar_site_url\n\nYou will be able to log into your site at !site_login_uri with username:\n\n!username\n\nWe hope you will enjoy your new web site.\n\nSincerely,\nThe !site Team", $msg_variables);
    } else {
        $body = t("Hi !username,\n\nYour new web site at !site has been activated.\n\nYour web site url is !scholar_site_url\n\nYou may now log in and reset your password by clicking on this link or copying and pasting it in your browser: \n\n !login_url \n\nThis login can be used only once. After logging in, you will be redirected to !scholar_edit_url so you can change your password. Once you have set your own password, you will be able to log in at !site_login_uri as username:\n\n!username\n\nWe hope you will enjoy your new web site.\n\nSincerely,\nThe !site Team", $msg_variables);
    }

    $params['subject'] = $subject;
    $params['body'] = $body;
    $result = drupal_mail('vsite_register', 'vsite_register_notify', $new_user->mail, $language, $params, $from, $send = TRUE);
    if ($result == FALSE) {
        drupal_set_message(t('Error sending notification mail to user.'), 'error');
    }
}

/**
 * Implements hook_mail().
 */
function vsite_register_mail($key, &$message, $params) {
    $message['subject'] = $params['subject'];
    $message['body'][] = $params['body'];

    //these values be used with any mail alter hooks
    $message['vsite'] = $params['vsite'];
    $message['user'] = $params['user'];
}


function create_siteinfo_node($username,$domain){
    $conn = ldap_connect('ldap.ucmerced.edu', 389);
    $bd = ldap_bind($conn, 'uid=Faculty Admin,ou=Special Users,dc=ucmerced,dc=edu', '@schebAg8');
    $dn = 'ou=People,dc=ucmerced,dc=edu';
    $filter = '(uid=' . $username . ')';
    $search = ldap_search($conn, $dn, $filter);
    $info = ldap_get_entries($conn, $search);
    if(count($info) > 0){
        $node = new stdClass();
        $node->type = 'site_info';
        node_object_prepare($node);
        $node->title = $entry;
        $node->field_site_name[LANGUAGE_NONE]['0']['url'] = $entry;
        $node->field_site_name[LANGUAGE_NONE]['0']['title'] = $info[0]['sn'][0] . ', ' . $info[0]['givenname'][0];
        $node->field_ucm_site_description[LANGUAGE_NONE]['0']['value'] = $info[0]['ou'][0];
        node_save($node);
    }
 }
