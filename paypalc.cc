#include <CkJsonObject.h>
#include <CkStringBuilder.h>
#include <CkRest.h>

void ChilkatSample(void)
    {
    // Note: Requires Chilkat v9.5.0.64 or greater.

    // This requires the Chilkat API to have been previously unlocked.
    // See Global Unlock Sample for sample code.

    // Load our previously obtained access token. (see PayPal OAuth2 Access Token)
    CkJsonObject jsonToken;
    jsonToken.LoadFile("qa_data/tokens/paypal.json");

    // Build the Authorization request header field value.
    CkStringBuilder sbAuth;
    // token_type should be "Bearer"
    sbAuth.Append(jsonToken.stringOf("token_type"));
    sbAuth.Append(" ");
    sbAuth.Append(jsonToken.stringOf("access_token"));

    // Make the initial connection.
    // A single REST object, once connected, can be used for many PayPal REST API calls.
    // The auto-reconnect indicates that if the already-established HTTPS connection is closed,
    // then it will be automatically re-established as needed.
    CkRest rest;
    bool bAutoReconnect = true;
    bool success = rest.Connect("api.sandbox.paypal.com",443,true,bAutoReconnect);
    if (success != true) {
        std::cout << rest.lastErrorText() << "\r\n";
        return;
    }

    // ----------------------------------------------------------------------------------------------
    // The code above this comment could be placed inside a function/subroutine within the application
    // because the connection does not need to be made for every request.  Once the connection is made
    // the app may send many requests..
    // ----------------------------------------------------------------------------------------------

    // Build the JSON containing the credit card data and transaction information.
    CkJsonObject json;
    json.put_EmitCompact(false);
    json.UpdateString("intent","sale");
    json.UpdateString("payer.payment_method","credit_card");
    json.UpdateString("payer.funding_instruments[0].credit_card.number","4032031087659974");
    json.UpdateString("payer.funding_instruments[0].credit_card.type","visa");
    // Do NOT include a leading zero.  For example, do not use "09" for the month.
    json.UpdateNumber("payer.funding_instruments[0].credit_card.expire_month","9");
    json.UpdateNumber("payer.funding_instruments[0].credit_card.expire_year","2021");
    json.UpdateNumber("payer.funding_instruments[0].credit_card.cvv2","111");
    json.UpdateString("payer.funding_instruments[0].credit_card.first_name","Joe");
    json.UpdateString("payer.funding_instruments[0].credit_card.last_name","Buyer");
    json.UpdateString("transactions[0].amount.total","7.47");
    json.UpdateString("transactions[0].amount.currency","USD");
    json.UpdateString("transactions[0].description","This is the payment transaction description.");

    std::cout << json.emit() << "\r\n";

    // The JSON created by the above code is this:

    // 	{ 
    // 	  "intent": "sale",
    // 	  "payer": { 
    // 	    "payment_method": "credit_card",
    // 	    "funding_instruments": [
    // 	      { 
    // 	        "credit_card": { 
    // 	          "number": "4032031087659974",
    // 	          "type": "visa",
    // 	          "expire_month": 9,
    // 	          "expire_year": 2021,
    // 	          "cvv2": 111,
    // 	          "first_name": "Joe",
    // 	          "last_name": "Buyer"
    // 	        }
    // 	      }
    // 	    ]
    // 	  },
    // 	  "transactions": [
    // 	    { 
    // 	      "amount": { 
    // 	        "total": "7.47",
    // 	        "currency": "USD"
    // 	      },
    // 	      "description": "This is the payment transaction description."
    // 	    }
    // 	  ]
    // 	}

    rest.AddHeader("Authorization",sbAuth.getAsString());
    rest.AddHeader("Content-Type","application/json");

    // Send the POST request containign the JSON in the request body, and get the JSON response.
    CkStringBuilder sbJsonRequest;
    json.put_EmitCompact(true);
    json.EmitSb(sbJsonRequest);
    CkStringBuilder sbJsonResponse;
    success = rest.FullRequestSb("POST","/v1/payments/payment",sbJsonRequest,sbJsonResponse);
    if (success != true) {
        std::cout << rest.lastErrorText() << "\r\n";
        return;
    }

    json.put_EmitCompact(false);
    json.LoadSb(sbJsonResponse);

    std::cout << "Response Status Code = " << rest.get_ResponseStatusCode() << "\r\n";

    // Did we get a 201 success response?
    if (rest.get_ResponseStatusCode() != 201) {
        std::cout << json.emit() << "\r\n";
        std::cout << "Failed." << "\r\n";
        return;
    }

    // Sample response JSON is shown below.
    std::cout << json.emit() << "\r\n";

    // Demonstrate how to access a few parts of the JSON response..
    std::cout << "cc number: " << json.stringOf("payer.funding_instruments[0].credit_card.number") << "\r\n";
    std::cout << "subtotal: " << json.stringOf("transactions[0].amount.details.subtotal") << "\r\n";
    std::cout << "refund href: " << json.stringOf("transactions[0].related_resources[0].sale.links[1].href") << "\r\n";

    // 	{ 
    // 	  "id": "PAY-66A12106PU3254228LA3BYKI",
    // 	  "create_time": "2016-11-23T22:46:01Z",
    // 	  "update_time": "2016-11-23T22:46:07Z",
    // 	  "state": "approved",
    // 	  "intent": "sale",
    // 	  "payer": { 
    // 	    "payment_method": "credit_card",
    // 	    "funding_instruments": [
    // 	      { 
    // 	        "credit_card": { 
    // 	          "type": "visa",
    // 	          "number": "xxxxxxxxxxxx9974",
    // 	          "expire_month": "9",
    // 	          "expire_year": "2021",
    // 	          "first_name": "Joe",
    // 	          "last_name": "Buyer"
    // 	        }
    // 	      }
    // 	    ]
    // 	  },
    // 	  "transactions": [
    // 	    { 
    // 	      "amount": { 
    // 	        "total": "7.47",
    // 	        "currency": "USD",
    // 	        "details": { 
    // 	          "subtotal": "7.47"
    // 	        }
    // 	      },
    // 	      "description": "This is the payment transaction description.",
    // 	      "related_resources": [
    // 	        { 
    // 	          "sale": { 
    // 	            "id": "70L88278E6781074B",
    // 	            "create_time": "2016-11-23T22:46:01Z",
    // 	            "update_time": "2016-11-23T22:46:07Z",
    // 	            "amount": { 
    // 	              "total": "7.47",
    // 	              "currency": "USD"
    // 	            },
    // 	            "state": "completed",
    // 	            "parent_payment": "PAY-66A12106PU3254228LA3BYKI",
    // 	            "links": [
    // 	              { 
    // 	                "href": "https://api.sandbox.paypal.com/v1/payments/sale/70L88278E6781074B",
    // 	                "rel": "self",
    // 	                "method": "GET"
    // 	              },
    // 	              { 
    // 	                "href": "https://api.sandbox.paypal.com/v1/payments/sale/70L88278E6781074B/refund",
    // 	                "rel": "refund",
    // 	                "method": "POST"
    // 	              },
    // 	              { 
    // 	                "href": "https://api.sandbox.paypal.com/v1/payments/payment/PAY-66A12106PU3254228LA3BYKI",
    // 	                "rel": "parent_payment",
    // 	                "method": "GET"
    // 	              }
    // 	            ],
    // 	            "fmf_details": {},
    // 	            "processor_response": { 
    // 	              "avs_code": "X",
    // 	              "cvv_code": "M"
    // 	            }
    // 	          }
    // 	        }
    // 	      ]
    // 	    }
    // 
    // 	  ],
    // 	  "links": [
    // 	    { 
    // 	      "href": "https://api.sandbox.paypal.com/v1/payments/payment/PAY-66A12106PU3254228LA3BYKI",
    // 	      "rel": "self",
    // 	      "method": "GET"
    // 	    }
    // 	  ]
    // 	}
    // 
    }